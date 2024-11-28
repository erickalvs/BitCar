/*
===========================================================================

Enunciado:
Este programa permite a gestao de um cadastro de veiculos, incluindo funcoes
como inserir veiculos, excluir veiculos, buscar veiculos, listar veiculos,
listar veiculos por combustivel, alterar preco de veiculo e calcular a media
de precos dos veiculos cadastrados.

===========================================================================
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// est carro
struct Carro
{
    int codigo;
    char marca[50];
    char modelo[50];
    int anoFabricacao;
    int anoModelo;
    char combustivel[20];
    char tipo[20];
    double precoVenda;
};

// Prototipo das funcoes

void exibirMenu();
void inserirCarro(int *quantidadeCarros, FILE *arquivo);
int verificarCodigo(FILE *arquivo, int codigo);
void excluirVeiculo(FILE *arquivo, int *quantidadeCarros);
void buscarCarro(FILE *arquivo, int codigo);
void listarCarros(FILE *arquivo);
void listarCarrosPorCombustivel(FILE *arquivo);
void alterarPrecoDeVeiculo(FILE *arquivo, int codigo, double novoPreco);
float mediaPreco(FILE *arquivo);

// Programa principal
int main(void)
{

    FILE *arquivo;
    arquivo = fopen("cadastro.bin", "rb+");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo. Tentando criar...\n");

        // Tenta criar o arquivo
        arquivo = fopen("cadastro.bin", "wb+");

        if (arquivo == NULL)
        {
            printf("Erro ao criar o arquivo.\n");
            return 1;
        }
    }

    int quantidadeCarros = 0;
    int opcao, codigo;
    double novoPreco;

    printf("=======================================================================\n");
    printf("Revenda de carros (BitCar)\n");
    printf("Autores: Erick Alves, Lucas Cintra\n");
    printf("Versao: 1.0.0\n");
    printf("=======================================================================\n");

    do
    {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            inserirCarro(&quantidadeCarros, arquivo);
            break;
        case 2:
            excluirVeiculo(arquivo, &quantidadeCarros);
            break;
        case 3:
            printf("Digite o codigo do carro: ");
            scanf("%d", &codigo);
            buscarCarro(arquivo, codigo);
            break;
        case 4:
            listarCarros(arquivo);
            break;
        case 5:
            printf("Informe o combustivel que deseja filtrar os veiculos: ");
            listarCarrosPorCombustivel(arquivo);
            break;
        case 6:
            printf("Informe o codigo do veiculo a ser alterado: ");
            scanf("%d", &codigo);
            printf("Informe o valor a ser alterado: ");
            scanf("%lf", &novoPreco);
            alterarPrecoDeVeiculo(arquivo, codigo, novoPreco);
            break;
        case 7:
            mediaPreco(arquivo);
            break;
        case 8:
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 8);

    fclose(arquivo);

    return 0;
};

// Funcao para exibir o menu
void exibirMenu()
{
    printf("\nMenu:\n");
    printf("1. Inserir um novo veiculo no cadastro.\n");
    printf("2. Excluir um veiculo do cadastro.\n");
    printf("3. Busca de um veiculo.\n");
    printf("4. Listagem de todos os veiculo com seus precos de venda.\n");
    printf("5. Listagem de todos os veiculos de um combustivel informado.\n");
    printf("6. Alteracao do preco de venda de um veiculo.\n");
    printf("7. Media de precos dos veiculo cadastrados.\n");
    printf("8. Parar o processamento.\n");
}

// Funcao para inserir um novo carro
void inserirCarro(int *quantidadeCarros, FILE *arquivo)
{
    fclose(arquivo);
    arquivo = fopen("cadastro.bin", "rb+");
    struct Carro novoCarro;

    printf("Codigo: ");
    scanf("%d", &novoCarro.codigo);

    if (verificarCodigo(arquivo, novoCarro.codigo))
    {
        printf("\nCodigo ja cadastrado.\n");
        return;
    }

    printf("Marca: ");
    scanf("%s", novoCarro.marca);

    printf("Modelo: ");
    scanf("%s", novoCarro.modelo);

    printf("Ano de fabricacao: ");
    scanf("%d", &novoCarro.anoFabricacao);

    printf("Ano do modelo: ");
    scanf("%d", &novoCarro.anoModelo);

    printf("Combustivel: ");
    printf("| 1-Gasolina | 2-Alcool | 3-Diesel | 4-Flex |\n");
    int opcao = -1;
    do
    {
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            strcpy(novoCarro.combustivel, "Gasolina");
            break;
        case 2:
            strcpy(novoCarro.combustivel, "Alcool");
            break;
        case 3:
            strcpy(novoCarro.combustivel, "Diesel");
            break;
        case 4:
            strcpy(novoCarro.combustivel, "Flex");
            break;
        default:
            printf("Opcao invalida!\n");
        }
    } while (opcao < 0 || opcao > 4);

    printf("Tipo: ");
    printf("| 1-Utilitario | 2-Passeio |\n");
    opcao = -1;
    do
    {
        scanf("%d", &opcao);
        if (opcao == 1)
        {
            strcpy(novoCarro.tipo, "Utilitario");
        }
        else if (opcao == 2)
        {
            strcpy(novoCarro.tipo, "Passeio");
        }
        else
            printf("Opcao invalida!");
    } while (opcao != 1 && opcao != 2);

    printf("Preco de venda: ");
    scanf("%lf", &novoCarro.precoVenda);

    // Grava o novo carro no arquivo
    fseek(arquivo, 0, SEEK_END);
    fwrite(&novoCarro, sizeof(struct Carro), 1, arquivo);

    // Atualiza o array de carros
    (*quantidadeCarros)++;

    printf("Veiculo cadastrado com sucesso.\n");
}

// Funcao verificar se existe veiculos com o codigo inserido ja casdastrado
int verificarCodigo(FILE *arquivo, int codigo)
{
    fclose(arquivo);
    arquivo = fopen("cadastro.bin", "rb+");
    fseek(arquivo, 0, SEEK_SET); // Volta para o inicio do arquivo

    struct Carro carroAtual;

    while (fread(&carroAtual, sizeof(struct Carro), 1, arquivo) == 1)
    {
        if (carroAtual.codigo == codigo)
        {
            return 1; // Codigo encontrado no arquivo
        }
    }

    return 0; // Codigo nao encontrado no arquivo
}

// Funcao para excluir um veiculo do cadastro
void excluirVeiculo(FILE *arquivo, int *quantidadeCarros)
{
    fclose(arquivo);
    arquivo = fopen("cadastro.bin", "rb+");
    int codigoExclusao = 0;
    printf("\nDigite o codigo do veiculo a ser excluido: ");
    scanf("%d", &codigoExclusao);

    FILE *tempFile = fopen("temp.bin", "wb+"); // Arquivo temporario para reescrever os dados

    fseek(arquivo, 0, SEEK_SET); // Volta para o inicio do arquivo
    struct Carro carroAtual;

    while (fread(&carroAtual, sizeof(struct Carro), 1, arquivo) == 1)
    {
        if (carroAtual.codigo != codigoExclusao)
        {
            fwrite(&carroAtual, sizeof(struct Carro), 1, tempFile);
        }
        else
        {
            (*quantidadeCarros)--; // Reduz o contador quando um veiculo e excluido
        }
    }

    fclose(arquivo);
    fclose(tempFile);

    remove("cadastro.bin");             // Remove o arquivo original
    rename("temp.bin", "cadastro.bin"); // Renomeia o arquivo temporario para o original

    printf("Veiculo excluido com sucesso!\n");
}

// Funcao Buscar Carro
void buscarCarro(FILE *arquivo, int codigo)
{
    fclose(arquivo);
    arquivo = fopen("cadastro.bin", "rb+");
    fseek(arquivo, 0, SEEK_SET); // Volta para o inicio do arquivo
    struct Carro carroAtual;
    int carroEncontrado = 0; // Adiciona uma variavel de controle
    while (fread(&carroAtual, sizeof(struct Carro), 1, arquivo) == 1)
    {
        if (carroAtual.codigo == codigo)
        {
            printf("\nDADOS DO CARRO:\n\n");
            printf("Marca: %s \n", carroAtual.marca);
            printf("Modelo: %s \n", carroAtual.modelo);
            printf("Ano: %d \n", carroAtual.anoFabricacao);
            printf("Combustivel: %s \n", carroAtual.combustivel);
            printf("Tipo: %s \n", carroAtual.tipo);
            printf("Valor venda: $%.2lf\n", carroAtual.precoVenda);
            carroEncontrado = 1; // Define a variavel como verdadeira
            break;
        }
    }
    if (carroEncontrado == 0) // Verifica se a variavel e falsa
    {
        printf("\nCarro nao encontrado.\n");
    }
}

// Listar todos veiculos com os valores
void listarCarros(FILE *arquivo)
{
    fclose(arquivo);
    arquivo = fopen("cadastro.bin", "rb+");
    fseek(arquivo, 0, SEEK_SET); // Volta para o inicio do arquivo
    struct Carro carroAtual;
    int contador = 0;
    while (fread(&carroAtual, sizeof(struct Carro), 1, arquivo) == 1)
    {
        printf("\nCarro %d -\n ", ++contador);
        printf("Marca: %s\n Modelo: %s\n Preco: $%.2f \n", carroAtual.marca, carroAtual.modelo,
               carroAtual.precoVenda);
    }
}

// Listar veiculos de acordo combustivel //
void listarCarrosPorCombustivel(FILE *arquivo)
{
    fclose(arquivo);
    arquivo = fopen("cadastro.bin", "rb+");

    int contador = 0;
    int opcao = -1;
    int carroEncontrado = 0;

    do
    {

        printf("\n| 1-Gasolina | 2-Alcool | 3-Diesel | 4-Flex |\n");
        scanf("%d", &opcao);
        char tipo[20];

        switch (opcao)
        {
        case 1:
            strcpy(tipo, "Gasolina");
            break;
        case 2:
            strcpy(tipo, "Alcool");
            break;
        case 3:
            strcpy(tipo, "Diesel");
            break;
        case 4:
            strcpy(tipo, "Flex");
            break;
        default:
            printf("Opcao invalida!\n");
            continue;
        }

        fseek(arquivo, 0, SEEK_SET); // Volta para o inicio do arquivo
        struct Carro carroAtual;
        while (fread(&carroAtual, sizeof(struct Carro), 1, arquivo) == 1)
        {
            if (!strcmp(carroAtual.combustivel, tipo))
            {
                printf("\nCarro %d\n- Marca: %s - Modelo: %s\n -Combustivel: %s - Preco: $%.2f\n", ++contador, carroAtual.marca, carroAtual.modelo,
                       carroAtual.combustivel, carroAtual.precoVenda);
                carroEncontrado = 1;
            }
        }

        if (!carroEncontrado)
        {
            printf("Nao existem veiculos com o combustivel selecionado.\n");
        }

    } while (opcao < 0 || opcao > 4);
}

// Alteracao do preco de venda de um veiculo sendo informado o seu codigo. Deve ser verificado se o veiculo informado existe no cadastro.
void alterarPrecoDeVeiculo(FILE *arquivo, int codigo, double novoPreco)
{
    fclose(arquivo);
    arquivo = fopen("cadastro.bin", "rb+");
    fseek(arquivo, 0, SEEK_SET); // Volta para o inicio do arquivo
    struct Carro carroAtual;
    int encontrado = 0;

    while (fread(&carroAtual, sizeof(struct Carro), 1, arquivo) == 1)
    {
        if (carroAtual.codigo == codigo)
        {
            encontrado = 1;
            carroAtual.precoVenda = novoPreco;
            fseek(arquivo, -sizeof(struct Carro), SEEK_CUR); // Volta para a posicao do carro atual no arquivo
            fwrite(&carroAtual, sizeof(struct Carro), 1, arquivo);
            printf("\nValor da venda atualizado com sucesso!\n");
            break;
        }
    }

    if (!encontrado)
        printf("\nO veiculo nao foi encontrado.\n");
}

// Calculo da media de precos dos carros cadastrados.
float mediaPreco(FILE *arquivo)
{
    fclose(arquivo);
    arquivo = fopen("cadastro.bin", "rb+");
    fseek(arquivo, 0, SEEK_SET); // Volta para o inicio do arquivo
    struct Carro carroAtual;
    float somaTotal = 0;
    int totalRegistros = 0;
    float mediaPreco;
    while (fread(&carroAtual, sizeof(struct Carro), 1, arquivo) == 1)
    {
        somaTotal += carroAtual.precoVenda;
        totalRegistros++;
    }
    mediaPreco = somaTotal / totalRegistros;
    printf("\nValor medio dos veiculos cadastrados : $%.2f\n", mediaPreco);

    return 0;
}