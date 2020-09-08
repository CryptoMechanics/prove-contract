project(prove)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( prove prove prove.cpp )
target_include_directories( prove PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( prove ${CMAKE_SOURCE_DIR}/../ricardian )