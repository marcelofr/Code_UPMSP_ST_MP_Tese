#!/bin/bash

cd ..

instance_folder="Instances/SMALL/";
#instance_folder="Instances/Debug/";
#instance_folder="Instances/LARGE/";

files="6_2_1439_3_S_1-9"
#files="*"
instance_extension=".dat"

#Sementes
seed[1]=60543
: '
seed[2]=16086
seed[3]=60048
seed[4]=10217
seed[5]=37451
seed[6]=7451
seed[7]=9330
seed[8]=55717
seed[9]=19025
seed[10]=40822
seed[11]=423
seed[12]=6241
seed[13]=26170
seed[14]=45433
seed[15]=27367
seed[16]=56890
seed[17]=54006
seed[18]=10667
seed[19]=61471
seed[20]=43978
seed[21]=42123
seed[22]=9653
seed[23]=55074
seed[24]=50396
seed[25]=44145
seed[26]=21001
seed[27]=34238
seed[28]=16792
seed[29]=31167
seed[30]=19162
'

#Pega quantas sementes serao executadas
size_seed=${#seed[@]}

#Algoritmos
algorithm[1]="SPEA2"
algorithm[2]="MOGA"
#algorithm[1]="EXACT"
#algorithm[1]="GA"
#algorithm[2]="MOVNS_Arroyo"
#algorithm[3]="MOVNS_Eduardo"
#algorithm[4]="MOVNS_D"

#Pega quantos algoritmos serao executados
size_algorithm=${#algorithm[@]}

folder_solution="Solutions/"

#tempo em milisegundos (valor sera multiplicado pela numero de tarefas da instancia)
max_time_factor[1]=1000
#Pega quantos tempos serao executados
size_max_time_factor=${#max_time_factor[@]}

if ! [ "$1" == "start" ] && ! [ "$1" == "restart" ]
then
    echo "opção inválida. digite: ./run.sh (start|restart)"
    exit 1
fi


#percorre os tempos size_max_time_factor
for((i=1;i<=$size_max_time_factor;i++))
do

    max_time_factor=${max_time_factor[$i]}

    #percorre os algoritmos
    for((j=1;j<=$size_algorithm;j++))
    do

        algorithm=${algorithm[$j]}


        #percorre todos os arquivos instance_extension
        find $instance_folder -maxdepth 1 -name $files$instance_extension -type f -print0 | while read -d $'\0' full_path_file; do

            filename="${full_path_file##*/}"
            instance_name="${filename%.[^.]*}"
            #echo "Nome da instância "$instance_name

            #percorre as sementes
            for((k=1;k<=$size_seed;k++))
            #for((k=1;k<=3;k++))
            #for((k=4;k<=$size_seed;k++))
            do

                seed=${seed[$k]}

                #Arquivo para salvar o logs de erros
                file_log_error="log/error/"$algorithm".log"

                #se nao existe a pasta para guardar os logs
                if ! [ -d "log/error/" ];
                then
                    #crie a pasta
                    mkdir -p "log/error/"
                    #echo "Criando pasta"
                fi

                #Arquivo para salvar o logs de situações
                file_log="log/status/"$algorithm".log"

                #se nao existe a pasta para guardar os logs
                if ! [ -d "log/status/" ];
                then
                    #crie a pasta
                    mkdir -p "log/status/"
                    #echo "Criando pasta"
                fi

                #se nao existe a pasta para guardar as solucoes do algoritmo
                if ! [ -d $folder_solution ];
                then
                    #crie a pasta
                    mkdir -p $folder_solution
                    #echo "Criando pasta"
                fi
                
                file_solution=$folder_solution$algorithm"_"$instance_name"_"$seed".sol"
                echo $file_solution
                if [ -f $file_solution ] && [ "$1" == "restart" ]
                then
                    echo "Arquivo "$file_solution" existente"
                    continue
                elif [ "$1" == "start" ]
                    then
                        echo "Start: $(date)"
                #    elif [ "$1" == "restart" ]
                #    then
                #        echo "Restart: $(date)"
                #    fi
                    echo "  instance_folder: "$instance_folder
                    echo "  instance: "$instance_name
                    echo "  extensao: "$instance_extension
                    echo "  seed: "$seed
                    echo "  algorithm: "$algorithm
                    echo "  max_time_factor: "$max_time_factor
                    echo "  folder_solution: "$folder_solution

                    destruction_factor=10
                    decomp_size=40
                    decomp_neigh_size=8

                    if [ $algorithm == "EXACT" ]
                    then
                        alpha=0.1
                        max_time_factor=$((max_time_factor / 1000))
                        echo "  new_max_time_factor: "$max_time_factor

                        for((k=1;k<=9;k++))
                        do

                            alpha_i=$(echo "scale=4; $alpha*$k" | bc)
                            echo "  alpha_i: "$alpha_i

                            ./build/src/src $instance_folder $instance_name $instance_extension $seed $algorithm $folder_solution $max_time_factor $alpha_i $prob_mutation
                        done

                        exit;
                        
                    fi

                    if [ $algorithm == "NSGAII" ]
                    then
                    	tam_population=110
	                    prob_mutation=5
                        echo "  tam_population: "$tam_population
                        echo "  prob_mutation: "$prob_mutation

                        ./build/src/src $instance_folder $instance_name $instance_extension $seed $algorithm $folder_solution $max_time_factor $tam_population $prob_mutation
                    fi
                    
                    if [ $algorithm == "SPEA2" ]
                    then
                    	tam_population=80
	                    prob_mutation=6
                        echo "  tam_population: "$tam_population
                        echo "  prob_mutation: "$prob_mutation

                        ./build/src/src $instance_folder $instance_name $instance_extension $seed $algorithm $folder_solution $max_time_factor $tam_population $prob_mutation
                    fi
                    
                    if [ $algorithm == "MOGA" ]
                    then
                    	tam_population=80
	                    prob_mutation=6
                        echo "  tam_population: "$tam_population
                        echo "  prob_mutation: "$prob_mutation

                        ./build/src/src $instance_folder $instance_name $instance_extension $seed $algorithm $folder_solution $max_time_factor $tam_population $prob_mutation
                    fi

                    if [ $algorithm == "MOVNS_Arroyo" ]
                    then
                        echo "  destruction_factor: "$destruction_factor

                        ./build/src/src $instance_folder $instance_name $instance_extension $seed $algorithm $folder_solution $max_time_factor $destruction_factor $prob_mutation
                    fi

                    if [ $algorithm == "MOVNS_D" ]
                    then
                        echo "  destruction_factor: "$destruction_factor

                        ./build/src/src $instance_folder $instance_name $instance_extension $seed $algorithm $folder_solution $max_time_factor $decomp_size $decomp_neigh_size
                    fi

                    #Se o comando falhou salva a hora
                    if [ $? -gt 0 ];
                    then
                        date "+%d/%m/%Y %H:%M:%S "$file_solution >> $file_log_error
                    else
                        #Se nao falhou escreve em outro log
                        date "+%d/%m/%Y %H:%M:%S Arquivo "$file_solution" Salvo com sucesso." >> $file_log
                    fi

                    echo "End: $(date)"
                    #date
                    echo ""
                
                fi #start
            done #Arquivos dat
        done #Sementes
    done #Algoritmos
done #tempos
