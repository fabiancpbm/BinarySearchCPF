/*
 * Neste programa eu faço a busca de um endereço específico através do CEP passado como argumento.
 * O algorítmo reproduz a busca é binária no arquivo de CEPs ordenados para encontrar o valor.
 * Ao final da leitura, é indicado o endereço caso este tenha sido encontrado. Caso contrário, uma mensagem é exibida
 * indicando que o endereço não foi encontrado.
 */

#include <stdio.h>
#include <mem.h>

/* Valor que define término do programa por motivos de erro no argumento passado para este programa. */
#define ARG_ERROR 1

/* Valor que define término do programa porque o arquivo não foi encontrado. */
#define FILE_NOT_FOUND 2

/* Erro de formato do CEP. */
#define CEP_FORMAT_ERROR 3

/*
 * Struct que representa uma unidade de endereço.
 */
struct _Address {
    char publicPlace[72];
    char district[72];
    char city[72];
    char uf[72];
    char initials[2];
    char cep[8];
    char trash[2];
} address;
typedef struct _Address Address;

/*
 * Função principal.
 */
int main(int argc, char **argv) {
    // Validando o argumento passado.
    if (argc != 2) {
        fprintf(stderr, "Uso correto: %s [CEP]", argv[0]);
        return ARG_ERROR;
    }

    // Validando o argumento do CEP que foi passado pelo usuário.
    int cepLength = strlen(argv[1]);
    if (cepLength != 8) {
        fprintf(stderr, "A quantidade de caracteres do CEP deve ser 8. Por favor, execute o programa novamente.");
        return CEP_FORMAT_ERROR;
    }

    // Capturando, validando e obtendo o tamanho do arquivo.
    FILE *f;
    f = fopen("cep_ordenado.dat", "r");
    if (f == NULL) {
        fprintf(stderr, "O arquivo nao foi encontrado. O programa sera fechado.");
        return FILE_NOT_FOUND;
    }
    long fileSize;
    fseek(f, 0, SEEK_END); // Indo par o final do arquivo.
    fileSize = ftell(f);
    rewind(f);

    // Obtendo o tamanho do arquivo em bytes.
    long firstIndex = 0;
    long lastIndex = (fileSize / sizeof(Address)) - 1;

    // Busca binária com loop.
    binarySearchByLoop(firstIndex, lastIndex, f, argv[1]);

    // Busca binária com recursão.
//    binarySearchByRecursion(0, lastIndex, f, argv[1]);

    // Fechando o arquivo.
    fclose(f);

    return 0;
}

/**
 * Busca binária com loop.
 *
 * @param firstIndex Primeiro índice para localizar o arquivo do meio.
 * @param lastIndex  Último índice para localizar o arquivo do meio.
 * @param f          Arquivo.
 * @param value      Valor procurado.
 */
void binarySearchByLoop(long firstIndex, long lastIndex, FILE* f, char**value) {
    while (firstIndex <= lastIndex) {
        long middle = (firstIndex + lastIndex) / 2;
        fseek(f, middle * sizeof(Address), SEEK_SET);
        fread(&address, sizeof(Address), 1, f);
        int result = strncmp(value, address.cep, 8);
        if (result == 0) {
            // Achei
            printf("\n%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n", address.publicPlace, address.district, address.city,
                   address.uf, address.initials,
                   address.cep);
            break;
        } else if (result > 0) {
            // O valor está depois.
            firstIndex = middle + 1;
        } else if (result < 0) {
            // O valor está antes.
            lastIndex = middle - 1;
        }
    }
    if (firstIndex > lastIndex) {
        fprintf(stderr, "Endereco nao encontrado.\n");
    }
}

/**
 * Busca binária com recursão.
 *
 * @param firstIndex Primeiro índice para localizar o arquivo do meio.
 * @param lastIndex  Último índice para localizar o arquivo do meio.
 * @param f          Arquivo.
 * @param value      Valor procurado.
 */
void binarySearchByRecursion(long firstIndex, long lastIndex, FILE *f, char **value) {
    long middle = (firstIndex + lastIndex) / 2;
    if (firstIndex <= lastIndex) {
        Address address;
        fseek(f, middle * sizeof(Address), SEEK_SET);
        fread(&address, sizeof(Address), 1, f);
        int result = strncmp(value, address.cep, 8);
        if (result == 0) {
            // Achei
            printf("\n%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n", address.publicPlace, address.district, address.city,
                   address.uf, address.initials,
                   address.cep);
            return;
        } else if (result > 0) {
            // O valor está depois.
            firstIndex = middle + 1;
        } else if (result < 0) {
            // O valor está antes.
            lastIndex = middle - 1;
        }
    } else {
        fprintf(stderr, "Endereco nao encontrado.\n");
        return;
    }
    binarySearchByRecursion(firstIndex, lastIndex, f, value);
}
