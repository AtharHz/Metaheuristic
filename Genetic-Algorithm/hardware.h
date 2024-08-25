#include "SystemC.h"
#include <iostream>
#include <cstdlib> 
#include <bits/stdc++.h>    
#include <array> 
using namespace std;

SC_MODULE(hardware) {

    sc_in<bool> clock;
    sc_in<int> totalW;
    sc_out<float> fitness;

    void start(){
        // cout << "hi\n";
        // int w=totalW.read();
        // cout<< "hello\n";
        // cout << "ine" << w;
        int w=10;
        bool same=false;
        int endCounter=0;
        float best[9]={0,0,0,0,0,0,0,0,0};
        float prevBest[9]={0,0,0,0,0,0,0,0,0};
        float var[9]={0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5};
        float probs[11]={0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};
        float newPopulation[20][9];
        int price[9]={6,5,8,9,6,7,3,6,8};
        int weight[9]={2,3,6,7,5,9,3,4,5};
        float population[10][9];
        int chroms=0;
        int counter=0;
        float arr[9];
        std::default_random_engine generator;
        std::normal_distribution<double> distribution(0.0,1.0);
        while(chroms<10) {
            for(int j = 0 ; j < 9 ; j++){
                float temp=((double) rand() / (RAND_MAX));
                // cout << "????" << temp << "\n";
                population[chroms][j]=temp;
                newPopulation[chroms][j]=temp;
                arr[j]=temp;
            }
            if(feasibility(weight,arr,w)){
                chroms++;
            }
        }

        for (int i = 10; i < 20 ; i++){
            for (int j = 0; j < 9 ; j++) { 
                newPopulation[i][j]=0;
            } 
        }

        for (int i = 0; i < 10 ; i++){
            for (int j = 0; j < 9 ; j++) { 
                // cout << population[i][j] << "\t     "; 
            } 
            // cout << "\n";
        }

        for(int j=0;j<50;j++){
            cout << "\ncounter :" << endCounter << endl;
            for(int i=0;i<9;i++){
                if(prevBest[i]!=best[i]){
                    endCounter=0;
                    same=false;
                    break;
                }
            }
            if(same){
                endCounter++;
            }else{
                same=true;
                for(int i=0;i<9;i++){
                    prevBest[i]=best[i];
                    endCounter=0;
                }
            }
            if(endCounter>5 && j>9){
                cout << "\n\n\nOVER\n" ;
                break;
            }else{
                iterate(population,price,weight,newPopulation,var,best,generator,distribution,w);
            }
        }
        float bestFit=0;
        for(int i=0;i<9;i++){
            bestFit+=price[i]*best[i];
        }
        fitness.write(bestFit);
        cout << "\nBest Fitness" << bestFit;
        return;
    }

	bool feasibility(int weight[9],float arr[9],int w){
        bool feasible=false;
        float totalWeight=0;
        for(int i = 0 ; i < 9 ; i++){
            totalWeight+=arr[i]*weight[i];
        }
        if(totalWeight<=w){
            feasible=true;
        }
        // cout << "w" << w <<"\n";
        // cout << "checking\n";
        // cout << totalWeight << "\n";
        // cout << feasible << "\n";
        return feasible;
    }

    void repair(float newPopulation[20][9],int weight[9] , int w){
        for(int j=0;j<20;j++){
            float totalWeight=0;
            for(int i = 0 ; i < 9 ; i++){
                totalWeight+=newPopulation[j][i]*weight[i];
            }
            if(totalWeight>w){
                for(int i = 0 ; i < 9 ; i++){
                    newPopulation[j][i]=0;
                }
            }
        }
    }

    void calculateFitness(float population[10][9],int price[9],int weight[9],float fitness[]){
        for(int i=0;i<10;i++){
            for(int j=0;j<9;j++){
                // fitness[i]+=population[i][j]*price[j]
                fitness[i]+=population[i][j]*price[j];
            }
        }
    }

    void sortingChoroms(float population[10][9],float fitness[]){
        float tempPop[10][9];
        float tempFit[10];
        for(int i=0;i<10;i++){
            tempFit[i]=fitness[i];
            // cout << "i " << i << "\n";
            // cout << "temp " << tempFit[i] << "\n";
            // cout << "fit " << fitness[i] << "\n";
            for(int j=0;j<9;j++){
                tempPop[i][j]=population[i][j];
            }
        }
        
        std::sort(tempFit, end(tempFit), std::greater<float>());
        // cout << "sorted\n";
        for(int i=0;i<10;i++){
            // cout<< tempFit[i] << "\t";
        }
        // cout << "\n";


        int num=0;
        while(num!=10){
            for(int i=0;i<10;i++){
                if(tempFit[num]==fitness[i]){
                    for(int j=0;j<9;j++){
                        population[num][j]=tempPop[i][j];
                        // cout << "here" ;
                        // cout << population[num][j] << "\n";
                    }
                    num++;
                    // cout<< "num" << num << "\t";
                    break;
                }
            }
        }

        // cout << "sorted pop\n";
        // for (int i = 0; i < 10 ; i++){
        //     for (int j = 0; j < 9 ; j++) { 
        //         cout << population[i][j] << "\t    "; 
        //     } 
        //     cout << "\n";
        // }

    }

    void mutation(float chorom[9],float vars[],default_random_engine generator,normal_distribution<double> distribution){
        
        double number = distribution(generator);
        // cout << "numbereeeeeee" << number << "\n";
        for(int j=0;j<9;j++){
            number = distribution(generator);
            chorom[j]+=vars[j]*number;
            if(chorom[j]<0){
                chorom[j]=0;
            }else if(chorom[j]>1){
                chorom[j]=1;
            }
            // cout << population[i][j] << "\t";
        }
    }

    void crossover(float firstPar[9],float secondPar[9],float newPopulation[20][9], float vars[], int newGeneration,default_random_engine generator,normal_distribution<double> distribution){
        int crossPoint=(rand() % 9);
        float firstChild[9];
        float secondChild[9];
        // cout << "printing the crossover point" << crossPoint<< "\n";
        for(int i=0;i<9;i++){
            if(i<crossPoint){
                newPopulation[10+newGeneration][i]=firstPar[i];
                newPopulation[10+newGeneration+1][i]=secondPar[i];
            }else{
                newPopulation[10+newGeneration][i]=secondPar[i];
                newPopulation[10+newGeneration+1][i]=firstPar[i];
            }
        }
        float pm=((double) rand() / (RAND_MAX));
        if(pm>0.2){
            mutation(newPopulation[10+newGeneration],vars,generator,distribution);
            mutation(newPopulation[10+newGeneration+1],vars,generator,distribution);
        }
        // cout << "new population\n";
        // for (int i = 0; i < 20 ; i++){
        //     for (int j = 0; j < 9 ; j++) { 
        //         cout << newPopulation[i][j] << "\t     "; 
        //     } 
        //     cout << "\n";
        // }
        newGeneration+=2;
        }


    void parentSelection(float population[10][9],float newPopulation[20][9],float vars[],int newGeneration,default_random_engine generator,normal_distribution<double> distribution){
        float firstPar[9];
        float secondPar[9];
        for(int i=0;i<5;i++){
            int chosen1=(rand() % 10);
            int chosen2=(rand() % 10);
            for(int j=0;j<9;j++){
                firstPar[j]=population[chosen1][j];
                secondPar[j]=population[chosen2][j];
            }
            // for(int i=0;i<9;i++){
            //     cout << firstPar[i] << "\t";
            // }
            // cout << "\n";
            // for(int i=0;i<9;i++){
            //     cout << secondPar[i] << "\t";
            // }
            // cout << "heeewwwy \n";
            // cout << "printing the base to see whether it has changed" << base << "\n";
            float pco=((double) rand() / (RAND_MAX));
            if(pco>0.1){
                crossover(firstPar,secondPar,newPopulation,vars,newGeneration,generator,distribution);
            }
        }
    }

    void calculateFitnessForNextGeneration(float newPopulation[20][9],int price[9],int weight[9],float fitness2[]){
        for(int i=0;i<20;i++){
            for(int j=0;j<9;j++){
                // fitness2[i]+=newPopulation[i][j]*price[j]/weight[j];
                fitness2[i]+=newPopulation[i][j]*price[j];
            }
        }
    }

    void sortAllChorms(float newPopulation[20][9],float fitness2[20]){
        float tempPop[20][9];
        float tempFit[20];
        for(int i=0;i<20;i++){
            tempFit[i]=fitness2[i];
            // cout << "i " << i << "\n";
            // cout << "temp " << tempFit[i] << "\n";
            // cout << "fit " << fitness2[i] << "\n";
            for(int j=0;j<9;j++){
                tempPop[i][j]=newPopulation[i][j];
            }
        }
        std::sort(tempFit, end(tempFit), std::greater<float>());
        // cout << "sorted\n";
        // for(int i=0;i<20;i++){
        //     cout<< tempFit[i] << "\t";
        // }


        int num=0;
        while(num!=20){
            for(int i=0;i<20;i++){
                if(tempFit[num]==fitness2[i]){
                    for(int j=0;j<9;j++){
                        newPopulation[num][j]=tempPop[i][j];
                        // cout << "here" ;
                        // cout << newPopulation[num][j] << "\n";
                    }
                    num++;
                    // cout<< "num" << num << "\t";
                    break;
                }
            }
        }

        // for (int i = 0; i < 20 ; i++){
        //     for (int j = 0; j < 9 ; j++) { 
        //         cout << newPopulation[i][j] << "\t     "; 
        //     } 
        //     cout << "\n";
        // }
        // std::sort(fitness2, end(fitness2), std::greater<float>());
    }

    void chooseNextGeneration(float newPopulation[20][9],float population[10][9]){
        // cout << "next generation\n";
        for(int i=0;i<10;i++){
            for(int j=0;j<9;j++){
                population[i][j]=newPopulation[i][j];
                // cout << population[i][j] << "\t     ";
            }
            // cout << "\n";
        }
    }

    void updateVariance(float vars[],default_random_engine generator,normal_distribution<double> distribution){
        // std::default_random_engine generator;
        // std::normal_distribution<double> distribution(0,1);
        double numCons = distribution(generator);
        for(int i=0;i<9;i++){
            double number=distribution(generator);
            vars[i]*=exp(numCons-number);
            if (vars[i] > 1){vars[i] = 1;}
            if (vars[i] < 0){vars[i] = 0;}
        }
    }

    void iterate(float population[10][9],int price[],int weight[],float newPopulation[20][9],float var[],float best[],default_random_engine generator,normal_distribution<double> distribution,int w){
        int newGeneration=0;
        float fitness[10]={0,0,0,0,0,0,0,0,0,0};
        float fitness2[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        cout << "\n iterate begin \n";
        calculateFitness(population,price,weight,fitness);
        // for (int j = 0; j < 10 ; j++) { 
        //     cout << fitness[j] << "\t"; 
        // } 
        // cout << "\n";

        sortingChoroms(population,fitness);
        parentSelection(population,newPopulation,var,newGeneration,generator,distribution);
        // cout << "new population\n";
        // for (int i = 0; i < 20 ; i++){
        //     for (int j = 0; j < 9 ; j++) { 
        //         cout << newPopulation[i][j] << "\t     "; 
        //     } 
        //     cout << "\n";
        // }
        // cout << "next step\n" ;
        newGeneration=0;
        repair(newPopulation,weight,w);
        calculateFitnessForNextGeneration(newPopulation,price,weight,fitness2);
        // cout << "Fitness :\n";
        // for (int j = 0; j < 20 ; j++) { 
        //     cout << fitness2[j] << "\t"; 
        // } 
        // cout << "\n";
        sortAllChorms(newPopulation,fitness2);
        chooseNextGeneration(newPopulation,population);
        updateVariance(var,generator,distribution);
        for(int i=0;i<9;i++){
            best[i]=population[0][i];
        }
        float temp=0;
        cout << "Best fit chormosom:\n";
        for(int i=0;i<9;i++){
            temp+=population[0][i]*price[i];
            cout << population[0][i] << "\t";
        }
        cout << "\n..................\n";
        cout << temp << "\n";
        cout << "..................\n";
    }

	SC_CTOR(hardware) {
		SC_METHOD(start);
		sensitive << totalW;
	}
};