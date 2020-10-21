#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <math.h>

#include <unistd.h>
#include <fcntl.h>

#define CURL_STATICLIB
#include <curl/curl.h>
#include <curl/easy.h>

/* 
// 内存形式
curl_formadd(&formpost, &lastptr, 
			CURLFORM_COPYNAME, "filename", 
			CURLFORM_BUFFER, "test.pdf", 
			CURLFORM_BUFFERPTR, data,
			CURLFORM_BUFFERLENGTH, data_len,
			CURLFORM_END);
*/

size_t url_write_data(void *data, size_t size, size_t nmemb, void *userp)
{
    memcpy(userp, data, size * nmemb);
	return size * nmemb;
}

void pdf2ofd(const char *url, const char *file, char *out)
{
    CURL *curl = NULL;  
    CURLcode res;  

    struct curl_httppost *formpost = NULL;  
    struct curl_httppost *lastptr = NULL;

    curl = curl_easy_init();

    if (curl) {  
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "filename", CURLFORM_FILE, file, CURLFORM_END);

        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, url_write_data);

	    curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK)  
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);  

        curl_formfree(formpost);  
    }  
}

const char *url_upload = "http://124.70.184.141/upload";
const char *pdf_file = "/Users/keming/p-pdf/pdftoofd/test/0000000005-1.pdf";

int main(int argc, char *argv[])
{
    char json[256] = { 0 };

    curl_global_init(CURL_GLOBAL_ALL);

    pdf2ofd(url_upload, pdf_file, json);

    curl_global_cleanup();

    printf("%s\n", json);

    return 0;
}


/*
http://url-addr/upload

// 返回值
{
	"status": 0,
	"data": "http://124.70.184.141/ofd-process/0050090931.ofd"
}
*/