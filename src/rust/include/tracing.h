// Copyright (c) 2020 Jack Grigg
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php .

#ifndef TRACING_INCLUDE_H_
#define TRACING_INCLUDE_H_

#include "rust/types.h"
#include "tracing/map.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#include <memory>

extern "C" {
#endif

struct TracingHandle;
typedef struct TracingHandle TracingHandle;

/// Initializes the tracing crate, returning a handle for the logging
/// component. The handle must be freed to close the logging component.
///
/// If log_path is NULL, logging is sent to standard output.
TracingHandle* tracing_init(
    const codeunit* log_path,
    size_t log_path_len,
    const char* initial_filter,
    bool log_timestamps);

/// Frees a tracing handle returned from `tracing_init`;
void tracing_free(TracingHandle* handle);

/// Reloads the tracing filter.
///
/// Returns `true` if the reload succeeded.
bool tracing_reload(TracingHandle* handle, const char* new_filter);

struct TracingCallsite;
typedef struct TracingCallsite TracingCallsite;

/// Creates a tracing callsite.
///
/// You should usually call the `TracingLog` macro (or one of the helper
/// macros such as `TracingInfo`) instead of calling this directly.
///
/// This MUST ONLY be called to assign a `static TracingCallsite*`, and all
/// string arguments MUST be static `const char*` constants, and MUST be valid
/// UTF-8.
TracingCallsite* tracing_callsite(
    const char* name,
    const char* target,
    const char* level,
    const char* file,
    uint32_t line,
    const char* const* fields,
    size_t fields_len,
    bool is_span);

struct TracingSpanHandle;
typedef struct TracingSpanHandle TracingSpanHandle;

struct TracingSpanGuard;
typedef struct TracingSpanGuard TracingSpanGuard;

/// Creates a span for a callsite.
///
/// The span must be freed when it goes out of scope.
TracingSpanHandle* tracing_span_create(
    const TracingCallsite* callsite,
    const char* const* field_values,
    size_t fields_len);

/// Clones the given span.
///
/// Both spans need to be separately freed when they go out of scope.
TracingSpanHandle* tracing_span_clone(const TracingSpanHandle* span);

/// Frees a span.
void tracing_span_free(TracingSpanHandle* span);

/// Enters the given span, returning a guard.
///
/// `tracing_span_exit` must be called to drop this guard before the span
/// goes out of scope.
TracingSpanGuard* tracing_span_enter(const TracingSpanHandle* span);

/// Exits a span by dropping the given guard.
void tracing_span_exit(TracingSpanGuard* guard);

/// Logs a message for a callsite.
///
/// You should usually call the `TracingLog` macro (or one of the helper
/// macros such as `TracingInfo`) instead of calling this directly.
void tracing_log(
    const TracingCallsite* callsite,
    const char* const* field_values,
    size_t fields_len);

#ifdef __cplusplus
}
#endif

//
// Helper macros
//

#define MAP_PAIR_LIST0(f, x, y, peek, ...) f(x, y) MAP_LIST_NEXT(peek, MAP_PAIR_LIST1)(f, peek, __VA_ARGS__)
#define MAP_PAIR_LIST1(f, x, y, peek, ...) f(x, y) MAP_LIST_NEXT(peek, MAP_PAIR_LIST0)(f, peek, __VA_ARGS__)

/// Applies the function macro `f` to each pair of the remaining parameters and
/// inserts commas between the results.
#define MAP_PAIR_LIST(f, ...) EVAL(MAP_PAIR_LIST1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

#define T_FIELD_NAME(x, y) x
#define T_FIELD_VALUE(x, y) y

#define T_FIELD_NAMES(...) MAP_PAIR_LIST(T_FIELD_NAME, __VA_ARGS__)
#define T_FIELD_VALUES(...) MAP_PAIR_LIST(T_FIELD_VALUE, __VA_ARGS__)

#define T_DOUBLEESCAPE(a) #a
#define T_ESCAPEQUOTE(a) T_DOUBLEESCAPE(a)

// Computes the length of the given array. This is COUNT_OF from Chromium.
#define T_ARRLEN(x) ((sizeof(x) / sizeof(0 [x])) / ((size_t)(!(sizeof(x) % sizeof(0 [x])))))

#ifdef __cplusplus
// Constructs a tracing callsite.
//
// The 'static constexpr' hack ensures that name, target, level, and fields are
// compile-time constants with static storage duration. The output of this macro
// MUST be stored as a static TracingCallsite*.
#define T_CALLSITE(name, target, level, fields, is_span) ([&] { \
    static constexpr const char* _t_name = name;                \
    static constexpr const char* _t_target = target;            \
    static constexpr const char* _t_level = level;              \
    static constexpr const char* const* _t_fields = fields;     \
    return tracing_callsite(                                    \
        _t_name,                                                \
        _t_target,                                              \
        _t_level,                                               \
        __FILE__,                                               \
        __LINE__,                                               \
        _t_fields,                                              \
        T_ARRLEN(fields),                                       \
        is_span);                                               \
}())
#else
// Constructs a tracing callsite.
//
// name, target, level, and fields MUST be static constants, and the output of
// this macro MUST be stored as a static TracingCallsite*.
#define T_CALLSITE(name, target, level, fields, is_span) \
    tracing_callsite(                                    \
        name,                                            \
        target,                                          \
        level,                                           \
        __FILE__,                                        \
        __LINE__,                                        \
        fields,                                          \
        T_ARRLEN(fields),                                \
        is_span)
#endif

//
// Spans
//

#ifdef __cplusplus
namespace tracing
{
class Span;

/// A guard representing a span which has been entered and is currently
/// executing.
///
/// When the guard is dropped, the span will be exited.
///
/// This is returned by the `Span::Enter` function.
class Entered
{
private:
    friend class Span;
    std::unique_ptr<TracingSpanGuard, decltype(&tracing_span_exit)> inner;

    Entered(const TracingSpanHandle* span) : inner(tracing_span_enter(span), tracing_span_exit) {}
};

/// A handle representing a span, with the capability to enter the span if it
/// exists.
///
/// If the span was rejected by the current `Subscriber`'s filter, entering the
/// span will silently do nothing. Thus, the handle can be used in the same
/// manner regardless of whether or not the trace is currently being collected.
class Span
{
private:
    std::unique_ptr<TracingSpanHandle, decltype(&tracing_span_free)> inner;

public:
    /// Constructs a span that does nothing.
    ///
    /// This constructor is present to enable spans to be stored in classes,
    /// while also containing fields that are initialized within the class
    /// constructor:
    ///
    ///     class Foo {
    ///         std::string name;
    ///         tracing::Span span;
    ///
    ///         Foo(std::string tag)
    ///         {
    ///             name = "Foo-" + tag;
    ///             span = TracingSpan("info", "main", "Foo", "name", name);
    ///         }
    ///     }
    Span() : inner(nullptr, tracing_span_free) {}

    /// Use the `TracingSpan` macro instead of calling this constructor directly.
    Span(const TracingCallsite* callsite, const char* const* field_values, size_t fields_len) : inner(tracing_span_create(callsite, field_values, fields_len), tracing_span_free) {}

    Span(Span& span) : inner(std::move(span.inner)) {}
    Span(const Span& span) : inner(tracing_span_clone(span.inner.get()), tracing_span_free) {}
    Span& operator=(Span& span)
    {
        if (this != &span) {
            inner = std::move(span.inner);
        }
        return *this;
    }
    Span& operator=(const Span& span)
    {
        if (this != &span) {
            inner.reset(tracing_span_clone(span.inner.get()));
        }
        return *this;
    }

    /// Enters this span, returning a guard that will exit the span when dropped.
    ///
    /// If this span is enabled by the current subscriber, then this function
    /// will call `Subscriber::enter` with the span's `Id`, and dropping the
    /// guard will call `Subscriber::exit`. If the span is disabled, this does
    /// nothing.
    Entered Enter()
    {
        return Entered(inner.get());
    }
};
} // namespace tracing

/// Expands to a `tracing::Span` object which is used to record a span.
/// The `Span::Enter` method on that object records that the span has been
/// entered, and returns a RAII guard object, which will exit the span when
/// dropped.
///
/// level, target, and name MUST be static constants, and MUST be valid UTF-8
/// strings.
#define TracingSpan(level, target, name) ([&] {        \
    static constexpr const char* const FIELDS[] = {};  \
    const char* T_VALUES[] = {};                       \
    static TracingCallsite* CALLSITE =                 \
        T_CALLSITE(name, target, level, FIELDS, true); \
    return tracing::Span(                              \
        CALLSITE, T_VALUES, T_ARRLEN(T_VALUES));       \
}())

/// Expands to a `tracing::Span` object which is used to record a span.
/// The `Span::Enter` method on that object records that the span has been
/// entered, and returns a RAII guard object, which will exit the span when
/// dropped.
///
/// Arguments: (level, target, name, key, value[, key2, value2, ...])
///
/// level, target, name, and all keys MUST be static constants, and MUST be
/// valid UTF-8 strings.
#define TracingSpanFields(level, target, name, ...) ([&] { \
    static constexpr const char* const FIELDS[] =          \
        {T_FIELD_NAMES(__VA_ARGS__)};                      \
    const char* T_VALUES[] =                               \
        {T_FIELD_VALUES(__VA_ARGS__)};                     \
    static TracingCallsite* CALLSITE =                     \
        T_CALLSITE(name, target, level, FIELDS, true);     \
    return tracing::Span(                                  \
        CALLSITE, T_VALUES, T_ARRLEN(T_VALUES));           \
}())
#endif

//
// Events
//

/// Constructs a new event.
///
/// level and target MUST be static constants, and MUST be valid UTF-8 strings.
#define TracingLog(level, target, ...)                       \
    do {                                                     \
        static constexpr const char* const FIELDS[] =        \
            {T_FIELD_NAMES("message", __VA_ARGS__)};         \
        const char* T_VALUES[] =                             \
            {T_FIELD_VALUES("message", __VA_ARGS__)};        \
        static TracingCallsite* CALLSITE = T_CALLSITE(       \
            "event " __FILE__ ":" T_ESCAPEQUOTE(__LINE__),   \
            target, level, FIELDS, false);                   \
        tracing_log(CALLSITE, T_VALUES, T_ARRLEN(T_VALUES)); \
    } while (0)

/// Constructs an event at the error level.
///
/// Arguments: (target, message)
///
/// target MUST be a static constant, and MUST be valid UTF-8 string.
#define TracingError(...) TracingLog("error", __VA_ARGS__)
/// Constructs an event at the warn level.
///
/// Arguments: (target, message)
///
/// target MUST be a static constant, and MUST be valid UTF-8 string.
#define TracingWarn(...) TracingLog("warn", __VA_ARGS__)
/// Constructs an event at the info level.
///
/// Arguments: (target, message)
///
/// target MUST be a static constant, and MUST be valid UTF-8 string.
#define TracingInfo(...) TracingLog("info", __VA_ARGS__)
/// Constructs an event at the debug level.
///
/// Arguments: (target, message)
///
/// target MUST be a static constant, and MUST be valid UTF-8 string.
#define TracingDebug(...) TracingLog("debug", __VA_ARGS__)
/// Constructs an event at the trace level.
///
/// Arguments: (target, message)
///
/// target MUST be a static constant, and MUST be valid UTF-8 string.
#define TracingTrace(...) TracingLog("trace", __VA_ARGS__)

#endif // TRACING_INCLUDE_H_
