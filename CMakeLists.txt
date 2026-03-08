cmake_minimum_required(VERSION 3.10)
project(practicas)

set(CMAKE_CXX_STANDARD 17)


# ============================================
# Biblioteca dinámica NO distribuida: libclaves.so
# ============================================

add_library(claves SHARED
        claves.c
        claves.h
)

target_compile_options(claves PRIVATE -fPIC)

# ============================================
# Biblioteca dinámica del PROXY: libproxyclaves.so
# ============================================

add_library(proxyclaves SHARED
        proxy-mq.c
        proxy-mq.h
        claves.h
)

target_compile_options(proxyclaves PRIVATE -fPIC)
target_link_libraries(proxyclaves PRIVATE rt pthread)

# ============================================
# Cliente NO distribuido
# ============================================

add_executable(cliente_local
        app-cliente.c
)

target_link_libraries(cliente_local PRIVATE claves)

# ============================================
# Servidor distribuido
# ============================================

add_executable(servidor_mq
        servidor-mq.c
        servidor-mq.h
)

target_link_libraries(servidor_mq PRIVATE claves pthread rt)

# ============================================
# Cliente DISTRIBUIDO
# ============================================

add_executable(cliente_mq
        app-cliente.c
)

target_link_libraries(cliente_mq PRIVATE proxyclaves)
