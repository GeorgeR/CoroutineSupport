#pragma once

#include "CoreMinimal.h"

#include "Async/Future.h"

#if !defined(WITH_CO_AWAIT)
	#if defined(_MSC_VER)
		#if (_MSC_FULL_VER >= 190023506)
			#if defined(_RESUMABLE_FUNCTIONS_SUPPORTED)
			#define WITH_CO_AWAIT 1
			#endif
		#endif
	#elif defined(__clang__)
		#if (__cpp_coroutines >= 201703)
			#if __has_include(<experimental/coroutine>)
			#define WITH_CO_AWAIT 1
			#endif
		#endif
	#else
	#define WITH_CO_AWAIT 0
	#endif
#endif

#if WITH_CO_AWAIT

namespace std {
	namespace experimental {
		template <typename ResultType, typename... Arguments>
		struct coroutine_traits<TFuture<ResultType>, Arguments...>
		{
			struct promise_type
			{
				TPromise<ResultType> _promise;
				TFuture<ResultType> get_return_object() { return _promise.GetFuture(); }

				auto initial_suspend() { return suspend_never{}; }
				auto final_suspend() { return suspend_never{}; }

				void set_exception(std::exception_ptr ex) { }

				template <typename U>
				void return_value(U&& value) { _promise.SetValue(Forward(Value)); }
			};
		};
	}
}

//template<typename ResultType>
//struct TFutureAwaiter
//{
//private:
//    TFuture<ResultType>& Future;
//
//public:
//    bool await_ready() { return Future.IsReady(); }
//
//    void await_suspend(std::experimental::coroutine_handle<> Handle)
//    {
//        Future.Then([Handle]() { Handle.resume(); });
//    }
//
//    auto await_resume() { return Future.Get(); }
//};
//
//template <typename ResultType>
//TFutureAwaiter<ResultType> operator co_await(TFuture<ResultType>& Value)
//{
//    return TFutureAwaiter<ResultType>{Value};
//}

template <typename ResultType>
auto operator co_await(TFuture<ResultType>&& Value)
{
	struct TFutureAwaiter
	{
		TFuture<ResultType>&& Input;
		TFuture<ResultType> Output;

		bool await_ready() { return false; }

		auto await_resume() { return Output.Get(); }

		void await_suspend(std::experimental::coroutine_handle<> Handle)
		{
			Input.Then([this, Handle](auto Result)
			{
				this->Output = Move(Result);
				Handle.resume();
			});
		}
	};

	return TFutureAwaiter{ StaticCast<TFuture<ResultType>&&>(Value); }
}

#endif
