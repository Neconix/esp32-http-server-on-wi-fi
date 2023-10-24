set(srcs 
        "main.c"
        "wifi.c"
        "http_server.c"
        "web_handlers/test_handler.c"
        "web_handlers/index_handler.c"
   )

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ".")
