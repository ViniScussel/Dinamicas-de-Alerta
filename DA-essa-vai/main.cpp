#include <iostream>

#include "classes/ambiente.h"

int main(){
    Ambiente env;
    env.povoar();
    for(int i=0; i<100000; i++){
        env.execucao();
    }
    return 0;
}