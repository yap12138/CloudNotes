#include <pthread.h>
#include <unistd.h>
#include <iostream>

pthread_key_t thread_atexit_key;
pthread_once_t thread_atexit_once = PTHREAD_ONCE_INIT;

class TData
{
	pthread_t _tid;
public:
	TData(pthread_t tid) : _tid(tid) { std::cout << _tid << " local create" << std::endl; }
	~TData() { std::cout << _tid << " exit" << std::endl; }
};

void thread_key_cleanup( void* arg )
{
	TData* p = static_cast<TData*>(arg);
	if (p != nullptr)
	{
		delete p;
	}
}

void* child( void* arg )
{
	if ( pthread_getspecific( thread_atexit_key ) == NULL)
	{
		TData* p = new TData( pthread_self() );
		pthread_setspecific( thread_atexit_key, p );
	}
	return nullptr;
}

void make_thread_atexit_key()
{
	if ( pthread_key_create( &thread_atexit_key, thread_key_cleanup ) != 0 )
	{
		abort();
	}
}

void protect_tls_clean()
{
	pthread_exit(0);
}

int main(int argc, char* argv[])
{
	pthread_t t1, t2;
	pthread_once( &thread_atexit_once, make_thread_atexit_key );

	pthread_create( &t1, NULL, child, NULL );
	pthread_create( &t2, NULL, child, NULL );

	// pthread_join( t1, NULL );
	// pthread_join( t2, NULL );

	pthread_detach( t1 );
	pthread_detach( t2 );

	// delete后key的清理函数得不到调用
	// pthread_key_delete( thread_atexit_key );

	// sleep( 1 );
	std::atexit( protect_tls_clean ); // 保证在main推出之前退出线程，保证tls被析构

	std::cout << "exit main" << std::endl;
	return 0;
}
