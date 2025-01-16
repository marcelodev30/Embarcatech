# Adicionar Bibliotecas no SDK do Pico (C/C++) no VS Code

Este repositório foi criado para ensinar, de maneira simples e prática, como adicionar bibliotecas no VS Code ao trabalhar com o Raspberry Pi Pico W utilizando o SDK em C.

## Passo 1: Identificar a Biblioteca Necessária

O SDK do Pico inclui várias bibliotecas. Exemplos comuns:
- **PWM**: `hardware_pwm`
- **SPI**: `hardware_spi`
- **I²C**: `hardware_i2c`

## Passo 2: Modificar o `CMakeLists.txt`

Procure a linha de comando `target_link_libraries` no arquivo `CMakeLists.txt`. Nesta linha, basta adicionar o nome da biblioteca que você deseja utilizar. 
:

#### Um Exemplo com `hardware_pwm` e `hardware_spi`:
```cmake
target_link_libraries(meu_projeto pico_stdlib hardware_pwm hardware_spi)
```
## Passo 3: Incluir os Cabeçalhos no Código

No arquivo src/main.c, inclua os cabeçalhos das bibliotecas que você deseja usar.
#### Exemplo:

``` c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h" // Biblioteca PWM
#include "hardware/spi.h" // Biblioteca SPI

```
## Passo 4: Atualizar o CMake e Compilar

No terminal, navegue até a pasta build (ou crie-a caso ainda não exista) e atualize o projeto com o CMake.

``` bash
cd build
cmake ..
make
```


## Recursos Adicionais
Para mais informações, consulte:

[Repositório do Pico SDK no GitHub](https://github.com/raspberrypi/pico-sdk)

