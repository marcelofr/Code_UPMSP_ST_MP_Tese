1. Mudar os parâmetros que serão testado no arquivo parameters.txt

tam_population " " c (80, 90, 100, 110)
prob_mutation " " c (4, 5, 6, 7)

destruction_factor " " c (5, 10, 20, 30, 40, 50, 60)


2. Mudar a configuração inicial dos parâmetros no arquivo configurations.txt

tam_population  prob_mutation
100 5

destruction_factor
5

3. Alterar o script do Irace que chama a execução do meu programa, ele precisa ter o parâmetro que está sendo testado. (target-runner)


4. Descomentar no meu código o #DEFINE IRACE e compilar o programa


5. Colocar as instâncias que serão testadas na pasta correspondente (tuning/Instances)

6. Verificar o nome da pasta onde está sendo salvas as soluções.

Atualmente, com o parâmetro do Irace (#DEFINE IRACE) o código não salva as soluções, apenas imprime os resultados de hipervolume na tela.

7. Executar o Irace.

irace --scenario scenario.txt --check
irace --scenario scenario.txt
------------------------------------------

sudo aptitude install r-base
https://www.rstudio.com/products/rstudio/download/#download
install.packages("irace")
irace --scenario scenario.txt --check
irace --scenario scenario.txt