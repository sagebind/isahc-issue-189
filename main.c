#include <pthread.h>
#include <curl/curl.h>

#define THREADS 8

// Ensure curl_global_init is called only once.
static pthread_once_t init = PTHREAD_ONCE_INIT;

// Does nothing with the response body.
static size_t write_callback(void *buffer, size_t size, size_t nmemb, void *userp)
{
   return size * nmemb;
}

static void init_routine()
{
    curl_global_init(CURL_GLOBAL_ALL);
}

static void *thread_main(void *arg)
{
    // This lazy initialization is basically what the curl crate does.
    pthread_once(&init, init_routine);

    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_perform(curl);

    // Double-free occurs somewhere inside here when LibreSSL cleanup is called.
    curl_easy_cleanup(curl);

    return NULL;
}

int main(int argc, char **argv)
{
    pthread_t tid[THREADS];

    // Spawn N threads. One of them will be first to initialize curl.
    for (int i = 0; i < THREADS; i++)
    {
        pthread_create(&tid[i], NULL, thread_main, NULL);
    }

    // Wait for all threads to return. We won't finish this if we SIGABRT.
    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(tid[i], NULL);
    }

    return 0;
}
