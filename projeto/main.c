#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cadCliente {
  char nome[50];
  char sexo;
  int idade;
};
typedef struct cadCliente Cliente;

struct cadVenda {
  Cliente cliente;
  int qtdItens;
  int hora, minuto;
  float valorTotal;
};
typedef struct cadVenda Venda;

int contarLinhaNome(FILE *p_a, int *cont, char *pesquisarNome);
void acessarLinhaNome(FILE *p_a, char *pesquisarNome, Venda *venda);

void mediaValor (int contVenda, Venda *venda, float *mediaTotalCliente);

void contarLinhaQtdAcima(FILE *p_a, int *cont, float *valorBase);
void contarVendasAposMD(FILE *p_a, int *cont);
void contarClientesF(FILE *p_a, int *cont);

void somaValorTotalM(FILE *p_a, float *soma);


void contaQtdItens(FILE *p_a, int *cont);
void valorTotalCompra(FILE *p_a, float *valor);
void exigeCompraCara(FILE *p_a, float *valor);
void exigeValorMedio(FILE *p_a,  float *precoMedio);
void exigeMenorNome(FILE *p_a, Venda *venda);
void pessoaMaisVelha(FILE *p_a, Venda *venda);

int main() {

  FILE *p_arquivo;
  int escolha;
  Venda *venda;
  int qtdVendas;
  int repetir;


  char pesquisarNome[50];
  char* pesquisa;
  int contVenda=0;
  float mediaTotalCliente;

  int cont = 0;
  float valor = 0;
  int contQtdAcima=0;
  float valorTotalVendas = 0;

  float valorBase;
  int contVendasTarde=0;
  int contMulher=0;
  float somaValorM=0;
  float precoMedio = 0;
  Venda menorNome;
  Venda maisVelha;

  do{

    do{
      printf("-------------------------------");
      printf("\n|          Menu:              |");
      printf("\n|1. Cadastrar novas vendas    |");
      printf("\n|2. Acessar uma venda         |");
      printf("\n|3. Acessar todas as vendas   |");
      printf("\n|4. Encerrar programa         |");
      printf("\n-------------------------------");

      printf("\nOpção: ");
      scanf("%d", &escolha);

    } while(escolha != 1 && escolha != 2 && escolha != 3 && escolha != 4);

    switch (escolha) {

    case 1:

      printf("\n-----------Cadastro de Vendas-----------\n");

      printf("Quantas vendas deseja cadastrar? ");
      scanf("%d", &qtdVendas);

      do{

        venda = (Venda *)malloc(qtdVendas * sizeof(Venda));
        if (venda == NULL) {
          printf("Erro! Memória insuficiente.");
          return 1;
        }

        for (int i = 0; i < qtdVendas; i++) {
          // CADASTRO DO CLIENTE
          printf("\n--CADASTRE O CLIENTE:\n");

          getchar();
          printf("Nome: ");
          fgets(venda[i].cliente.nome, 50, stdin);
          venda[i].cliente.nome[strcspn(venda[i].cliente.nome, "\n")] = '\0';
          while (strlen(venda[i].cliente.nome) < 4) {
            printf("\nErro! Nome inválido.");

            printf("\n(O nome deve ter, no mínimo, 4 dígitos)\n");
            printf("Nome: ");
            fgets(venda[i].cliente.nome, 50, stdin);

            venda[i].cliente.nome[strcspn(venda[i].cliente.nome, "\n")] = '\0';
          }

          printf("Sexo: ");
          scanf("%c", &venda[i].cliente.sexo);
          getchar();

          while (venda[i].cliente.sexo != 'm' && venda[i].cliente.sexo != 'f' &&
                 venda[i].cliente.sexo != 'n') {
            printf("\nErro! Sexo inválido.");

            printf("\n(Digite: "
                   "m"
                   ", "
                   "f"
                   " ou "
                   "n"
                   ")\n");
            printf("Sexo: ");
            scanf("%c", &venda[i].cliente.sexo);
            getchar();
          }

          printf("Idade: ");
          scanf("%d", &venda[i].cliente.idade);
          getchar();

          // CADASTRO DA COMPRA
          printf("\n--CADASTRE A COMPRA:\n");

          printf("Quantidade de Itens: ");
          scanf("%d", &venda[i].qtdItens);
          getchar();

          printf("- Horário da Compra");
          printf("\nDigite a hora: ");

          scanf("%d", &venda[i].hora);
          getchar();

          printf("Digite os minutos | %d:", venda[i].hora);

          scanf("%d", &venda[i].minuto);
          getchar();

          printf("Valor Total: R$");
          scanf("%f", &venda[i].valorTotal);
          getchar();

          while (venda[i].valorTotal <= 0) {

            printf("\nErro! Valor de compra inválido.");

            printf("\n(Digite um valor positivo)\n");
            printf("Valor Total: R$");

            scanf("%f", &venda[i].valorTotal);
            getchar();
          }

          p_arquivo = fopen("registro.txt", "a");

          fprintf(p_arquivo, "%s|%c|%d|%d|%d:%d|%.2f\n", venda[i].cliente.nome, venda[i].cliente.sexo, venda[i].cliente.idade, venda[i].qtdItens, venda[i].hora, venda[i].minuto, venda[i].valorTotal);
        }

        free(venda);
        fclose(p_arquivo);

        printf("\nVenda Cadastrada com Sucesso!");
        printf("\n\n");

        printf("Escolha:");
        printf("\n1 - Cadastrar %d venda(s) novamente.", qtdVendas);
        printf("\n2 - Voltar ao Menu.");

        printf("\n\nR: ");
        scanf("%d", &repetir);

        if(repetir != 1 && repetir !=2){
          printf("Escolha Inválida! Digite 1 ou 2\n\n");

          printf("Escolha:");
          printf("\n1 - Cadastrar %d venda(s) novamente.", qtdVendas);
          printf("\n2 - Voltar ao Menu.");
        }
      } while(repetir == 1);

    break;

    case 2:

      do{

        printf("\n--Pesquisa de Venda--\n");

        getchar();
        printf("Nome do Cliente: ");
        fgets(pesquisarNome, 50, stdin);

        pesquisarNome[strcspn(pesquisarNome, "\n")] = 0;

        p_arquivo = fopen("registro.txt","r");
        if(p_arquivo == NULL){
          printf("Erro! Não foi possível abrir o arquivo.");
          return 1;
        }

        contarLinhaNome(p_arquivo, &contVenda, pesquisarNome);

        fclose(p_arquivo);

        if(contVenda == 0){
          printf("\nNão há compras nesse nome!");
        }

        else{
          venda = (Venda *)malloc(contVenda * sizeof(Venda));

          if (venda == NULL){
            printf("Erro! Memória insuficiente.");
            return 1;
          }

          p_arquivo = fopen("registro.txt","r");
          if(p_arquivo == NULL){
            printf("Erro! Não foi possível abrir o arquivo.");
            return 1;
          }

          acessarLinhaNome(p_arquivo, pesquisarNome, venda);

          for(int i=0; i<contVenda; i++){

            if(i == 0){
              printf("Dados de %s:\n\n", venda[i].cliente.nome);
              printf("Sexo: %c\n", venda[i].cliente.sexo);
              printf("Idade: %d\n", venda[i].cliente.idade);
            }
            printf("\n%dº Compra:\n\n", i+1);
            printf("Quantidade de Itens: %d\n", venda[i].qtdItens);
            printf("Horário: %d:%d\n", venda[i].hora, venda[i].minuto);
            printf("Valor: %.2f\n", venda[i].valorTotal);
          }

          mediaValor(contVenda, venda, &mediaTotalCliente);
          printf("\nValor médio das compras = %.2f\n\n", mediaTotalCliente);

          printf("Pesquisa concluída com sucesso!\n");

          fclose(p_arquivo);
          free(venda);
        }

        printf("\nEscolha:");
        printf("\n1 - Pesquisar novamente.");
        printf("\n2 - Voltar ao Menu.");

        printf("\n\nR: ");
        scanf("%d", &repetir);

        if(repetir != 1 && repetir !=2){
          printf("Escolha Inválida! Digite 1 ou 2\n\n");

          printf("Escolha:");
          printf("\n1 - Pesquisar novamente.");
          printf("\n2 - Voltar ao Menu.");
        }
      } while(repetir == 1);

    break;

    case 3:

      printf("\n-----------Dados das Vendas realizadas-----------\n\n");

      // ● 1
      printf("Digite um valor para ver quantas compras são mais caras: ");
      scanf("%f", &valorBase);

      p_arquivo = fopen("registro.txt", "r");
      if(p_arquivo == NULL){
        printf("Erro! Não foi possível abrir o arquivo.");
        return 1;
      }

      contarLinhaQtdAcima(p_arquivo, &contQtdAcima, &valorBase);

      fclose(p_arquivo);

      printf("\n%d vendas acima de R$%.2f!\n", contQtdAcima, valorBase);

      printf("\n-----------Mais informações:------------\n");

      /*● 2 Exibir a quantidade de vendas que teve exatamente 2 itens vendidos.*/

      // ● 3 Exibir a quantidade de vendas que foram realizadas após o meio-dia.

      p_arquivo = fopen("registro.txt", "r");
      if(p_arquivo == NULL){
        printf("Erro! Não foi possível abrir o arquivo.");
        return 1;
      }

      contarVendasAposMD(p_arquivo, &contVendasTarde);

      fclose(p_arquivo);

      printf("\n%d vendas feitas após 12h!\n", contVendasTarde);

      // ● 4 Exibir a quantidade de compradoras mulheres.

      p_arquivo = fopen("registro.txt", "r");
      if(p_arquivo == NULL){
        printf("Erro! Não foi possível abrir o arquivo.");
        return 1;
      }

      contarClientesF(p_arquivo, &contMulher);

      fclose(p_arquivo);

      printf("\n%d clientes são mulheres!\n",contMulher);

      // ● 5 Exibir o valor total comprado por homens.
      p_arquivo = fopen("registro.txt", "r");
      if(p_arquivo == NULL){
        printf("Erro! Não foi possível abrir o arquivo.");
        return 1;
      }

      somaValorTotalM(p_arquivo, &somaValorM);

      fclose(p_arquivo);

      printf("\nOs homens gastaram R$%.2f!\n", somaValorM);

      // ● 6 Exibir a quantidade total de itens vendidos.
        p_arquivo = fopen("registro.txt", "r");
      if(p_arquivo == NULL){
        printf("Erro! Não foi possível abrir o arquivo.");
        return 1;
      }

      contaQtdItens(p_arquivo, &cont);

      fclose(p_arquivo); 

      printf("\nA quantidade total de itens é %d!\n", cont);

      // 7 Exibir o valor total das compras.
       p_arquivo = fopen("registro.txt", "r");
      if(p_arquivo == NULL){
        printf("Erro! Não foi possível abrir o arquivo.");
        return 1;
      }

      valorTotalCompra(p_arquivo, &valorTotalVendas);

      fclose(p_arquivo); 

      printf("\nO valor total das compras é R$%.2f\n", valorTotalVendas);

      //8 Exibir o valor médio de uma compra.
      p_arquivo = fopen("registro.txt", "r");
      if(p_arquivo == NULL){
        printf("Erro! Não foi possível abrir o arquivo.");
        return 1;
      }
      exigeValorMedio(p_arquivo, &precoMedio);

      fclose(p_arquivo); 

      printf("\nA média dos valores das compras é R$%.2f\n", precoMedio);

      // 9 Exibir o valor de compra mais cara.
       p_arquivo = fopen("registro.txt", "r");
      if(p_arquivo == NULL){
        printf("Erro! Não foi possível abrir o arquivo.");
        return 1;
      }
      exigeCompraCara(p_arquivo, &valor);

       fclose(p_arquivo); 

      printf("\nO valor da compra mais cara é R$%.2f!\n", valor);

      printf("\nEscolha:");
      printf("\n1 - Encerrar o programa.");
      printf("\n2 - Voltar ao Menu.");

      printf("\n\nR: ");
      scanf("%d", &repetir);

      if(repetir == 1){
        printf("\nO programa foi encerrado!");
        return 1;
      }

      else if(repetir != 1 && repetir !=2){
        printf("Escolha Inválida! Digite 1 ou 2\n\n");

        printf("Escolha:");
        printf("\n1 - Encerrar o programa.");
        printf("\n2 - Voltar ao Menu.");

        printf("\n\nR: ");
        scanf("%d", &repetir);
      }

    break;

    case 4:

      printf("O programa foi encerrado!\n\n");
      return 1;

    break;

    default:

      printf("\nEssa opção é inválida!\n\n");
      repetir = 2;
    }

    getchar();

  } while(repetir == 2);

  free(p_arquivo);

  return 0;
}

int contarLinhaNome(FILE *p_a, int *cont, char *pesquisarNome){

  *cont = 0;
  char linha[100];

  while(fgets(linha, 100, p_a) != NULL) {

    char *pesquisa = strtok(linha, "|");

    while(pesquisa != NULL){
      if(strcmp(pesquisarNome, pesquisa) == 0){
        (*cont)++;
      }

      pesquisa = strtok(NULL, "|");
    }
  }

  return 0;
}

void acessarLinhaNome(FILE *p_a, char *pesquisarNome, Venda *venda){

  char linha[100];

  char nomeLinha[50];
  char sexo;
  int idade, qtdItens, hora, minuto;
  float valorTotal;
  int i=0;

  while(fgets(linha, 100, p_a) != NULL) {

    sscanf(linha, "%[^|]|%c|%d|%d|%d:%d|%f\n", nomeLinha, &sexo, &idade, &qtdItens, &hora, &minuto, &valorTotal);

    if (strcmp(nomeLinha, pesquisarNome) == 0) { 

      strcpy((venda+i)->cliente.nome, nomeLinha);
      (venda+i)->cliente.sexo = sexo;
      (venda+i)->cliente.idade = idade;
      (venda+i)->qtdItens = qtdItens;
      (venda+i)->hora = hora;
      (venda+i)->minuto = minuto;
      (venda+i)->valorTotal = valorTotal;

      i++;
    }
  }
}

void mediaValor (int contVenda, Venda *venda, float *mediaTotalCliente){

  float somaValor=0;

  for(int i=0; i<contVenda; i++){
    somaValor += (venda+i)->valorTotal;
  }

  *mediaTotalCliente = somaValor / contVenda;
}

void contarLinhaQtdAcima(FILE *p_a, int *cont, float *valorBase){

  char linha[100];

  char nomeLinha[50];
  char sexo;
  int idade, qtdItens, hora, minuto;
  float valorTotal;

  while(fgets(linha, 100, p_a) != NULL) {

    sscanf(linha, "%[^|]|%c|%d|%d|%d:%d|%f\n", nomeLinha, &sexo, &idade, &qtdItens, &hora, &minuto, &valorTotal);

    if(*valorBase < valorTotal){
      (*cont)++;
    }
  }
}

void contarVendasAposMD(FILE *p_a, int *cont){

  char linha[100];

  char nomeLinha[50];
  char sexo;
  int idade, qtdItens, hora, minuto;
  float valorTotal;

  while(fgets(linha, 100, p_a) != NULL) {

    sscanf(linha, "%[^|]|%c|%d|%d|%d:%d|%f\n", nomeLinha, &sexo, &idade, &qtdItens, &hora, &minuto, &valorTotal);

    if(hora > 12) {
      (*cont)++;
    }
  }
}

void contarClientesF(FILE *p_a, int *cont){

  char linha[100];

  char nomeLinha[50];
  char sexo;
  int idade, qtdItens, hora, minuto;
  float valorTotal;

  while(fgets(linha, 100, p_a) != NULL) {

    sscanf(linha, "%[^|]|%c|%d|%d|%d:%d|%f\n", nomeLinha, &sexo, &idade, &qtdItens, &hora, &minuto, &valorTotal);

    if(sexo == 'f' || sexo == 'F') {
      (*cont)++;
    }
  }
}
//Exibir o valor total comprado por homens.
void somaValorTotalM(FILE *p_a, float *soma){

  char linha[100];

  char nomeLinha[50];
  char sexo;
  int idade, qtdItens, hora, minuto;
  float valorTotal;

  while(fgets(linha, 100, p_a) != NULL) {

    sscanf(linha, "%[^|]|%c|%d|%d|%d:%d|%f\n", nomeLinha, &sexo, &idade, &qtdItens, &hora, &minuto, &valorTotal);

    if(sexo == 'm' || sexo == 'M') {

      (*soma) += valorTotal;
    }
  }
}
//● Exibir a quantidade total de itens vendidos.
void contaQtdItens(FILE *p_a, int *cont){
    char linha[100];

  char nomeLinha[50];
  char sexo;
  int idade, qtdItens, hora, minuto;
  float valorTotal;

  while(fgets(linha, 100, p_a) != NULL) {

    sscanf(linha, "%[^|]|%c|%d|%d|%d:%d|%f\n", nomeLinha, &sexo, &idade, &qtdItens, &hora, &minuto, &valorTotal);

      (*cont) += qtdItens;
  }
}

//● Exibir o valor total das compras.
void valorTotalCompra(FILE *p_a, float *valor) {
    char linha[100];

  char nomeLinha[50];
  char sexo;
  int idade, qtdItens, hora, minuto;
  float valorTotal;

    while(fgets(linha, 100, p_a) != NULL) {

    sscanf(linha, "%[^|]|%c|%d|%d|%d:%d|%f\n", nomeLinha, &sexo, &idade, &qtdItens, &hora, &minuto, &valorTotal);

    (*valor) += valorTotal;

  }
}
//Exibir o valor médio de uma compra.
void exigeValorMedio(FILE *p_a, float *precoMedio) {
    char linha[100];

  char nomeLinha[50];
  char sexo;
  int idade, qtdItens, hora, minuto;
  float valorTotal;
  float totalCompra= 0;
  int totalVenda= 0;

    contaQtdItens(p_a, &totalVenda);
    rewind(p_a);
    valorTotalCompra(p_a, &totalCompra);

    (*precoMedio)= totalCompra/(float)totalVenda;
}
//Exibir o valor de compra mais cara.
void exigeCompraCara(FILE *p_a, float *valor) {
    char linha[100];

  char nomeLinha[50];
  char sexo;
  int idade, qtdItens, hora, minuto;
  float valorTotal;
  float compraCara= 0;

    while(fgets(linha, 100, p_a) != NULL) {

    sscanf(linha, "%[^|]|%c|%d|%d|%d:%d|%f\n", nomeLinha, &sexo, &idade, &qtdItens, &hora, &minuto, &valorTotal);

    if(valorTotal> compraCara) {
      compraCara= valorTotal;
    }
  }

  (*valor) = compraCara;
}
//Exibir os dados da venda do cliente com o menor nome (se ele fez mais
//de uma compra, exibir os dados de cada venda).
void exigeMenorNome(FILE *p_a, Venda *venda) {

      char linha[100];
      int nomeMenor= 100;
      char nomeLinha[50];
      char sexo;
      int idade, qtdItens, hora, minuto;
      float valorTotal;

    while(fgets(linha, 100, p_a) != NULL) {

    sscanf(linha, "%[^|]|%c|%d|%d|%d:%d|%f\n", nomeLinha, &sexo, &idade, &qtdItens, &hora, &minuto, &valorTotal);

    if(strlen(nomeLinha)< nomeMenor){
        nomeMenor= strlen(nomeLinha);

        strcpy((venda)->cliente.nome, nomeLinha);
      (venda)->cliente.sexo = sexo;
      (venda)->cliente.idade = idade;
      (venda)->qtdItens = qtdItens;
      (venda)->hora = hora;
      (venda)->minuto = minuto;
      (venda)->valorTotal = valorTotal;
    }
  }
}

//Exibir os dados da venda do cliente mais velho (se ele fez mais de uma
//compra, exibir os dados de cada venda).

void pessoaMaisVelha(FILE *p_a, Venda *venda) {
    char linha[100];

      int menorIdade= 0;
      char nomeLinha[50];
      char sexo;
      int idade, qtdItens, hora, minuto;
      float valorTotal;
      int i=0;

    while(fgets(linha, 100, p_a) != NULL) {

    sscanf(linha, "%[^|]|%c|%d|%d|%d:%d|%f\n", nomeLinha, &sexo, &idade, &qtdItens, &hora, &minuto, &valorTotal);

    if(idade > menorIdade){
        menorIdade= idade;

        strcpy((venda+i)->cliente.nome, nomeLinha);
      (venda+i)->cliente.sexo = sexo;
      (venda+i)->cliente.idade = idade;
      (venda+i)->qtdItens = qtdItens;
      (venda+i)->hora = hora;
      (venda+i)->minuto = minuto;
      (venda+i)->valorTotal = valorTotal;

      i++;
    }
  }
}