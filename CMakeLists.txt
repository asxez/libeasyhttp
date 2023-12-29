
add_library(easy-http easyhttp/easy-http.c)

find_package(CURL REQUIRED)
target_link_libraries(easy-http PRIVATE CURL::libcurl)
