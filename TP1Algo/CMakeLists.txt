cmake_minimum_required(VERSION 3.20)
project(TP1Algo)

# Configuration des chemins SDL
set(SDL3_DIR "F:/Documents/UQAT/algoJeux/TP/SDL3/SDL3-3.2.26/cmake")
set(SDL3_ttf_DIR "F:/Documents/UQAT/algoJeux/TP/SDL3/SDL3_ttf-3.2.2/cmake")

# Trouver les bibliothèques SDL
find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)

# Créer l'exécutable avec tous vos fichiers sources
add_executable(TP1Algo 
    TP1Algo.cpp
    # Ajoutez vos autres .cpp ici si nécessaire
)

# Lier les bibliothèques SDL
target_link_libraries(TP1Algo PRIVATE 
    SDL3::SDL3 
    SDL3_ttf::SDL3_ttf
)