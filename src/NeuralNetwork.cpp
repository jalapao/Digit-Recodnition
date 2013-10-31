#include <NeuralNetwork.h>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <cstring>

using namespace std;

Node hiddenLayers[10][layers];
Node inputNode[81];
LastNode lastHiddenLayer[10];
double outputNode[10];
double alpha = 0.5;

double sigmoidAdjust(double t)
{
    //return 2/(1+exp(-monkey/6))-1;
    return 1/(1+exp(-t/3));
}

void clearContent()
{
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<layers; j++)
        {
            hiddenLayers[i][j].output = 0.0;
        }
    }

    for(int k=0; k<10; k++)
    {
        outputNode[k] = 0.0;
    }
}

double* NeuralNetworkCompare(const NG_RESULT &ia)
{
    clearContent();
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            double density = ia[i][j];
            //inputNode[i*9+j].output = sigmoidAdjust(density);
            inputNode[i*9+j].output = density;
            //fprintf(stderr, "%lf ",density);
            //fprintf(stderr, "\n");
        }
    }
    fprintf(stderr, "NeuralNetwork init just fine.\n");

    if(layers!=0) //if there's a middle layer except the last middle layer
    {
        for(int pLayer=0; pLayer< layers; pLayer++)
        {
            for(int pRow =0; pRow<10; pRow++)
            {
                if(pLayer == 0)  //interact with input layer
                {
                    for(int inputIndex = 0; inputIndex < 81; inputIndex++)
                    {
                        hiddenLayers[pRow][pLayer].output += inputNode[inputIndex].weight[pRow] * inputNode[inputIndex].output;
                    }
                    hiddenLayers[pRow][pLayer].output = sigmoidAdjust(hiddenLayers[pRow][pLayer].output);
                }
                else  //interact with last layer
                {
                    for(int lastIndex=0; lastIndex<10; lastIndex++)
                    {
                        hiddenLayers[pRow][pLayer].output += hiddenLayers[lastIndex][pLayer-1].weight[pRow] * hiddenLayers[lastIndex][pLayer-1].output;
                    }
                    hiddenLayers[pRow][pLayer].output = sigmoidAdjust(hiddenLayers[pRow][pLayer].output);
                }
                fprintf(stderr, "%lf ",hiddenLayers[pRow][pLayer].output);
                fprintf(stderr, "\n");
            }
        }  //put values in every node of middle layers
    }
    fprintf(stderr, "NeuralNetwork middle layers.\n");

    for(int pRow=0; pRow<10; pRow++)
    {
        for(int lastIndex = 0; lastIndex<10; lastIndex++)
        {
            lastHiddenLayer[pRow].output += hiddenLayers[lastIndex][layers-1].weight[pRow] * hiddenLayers[lastIndex][layers-1].output;
        }
        lastHiddenLayer[pRow].output = sigmoidAdjust(lastHiddenLayer[pRow].output);
    }//put values in last layer
    fprintf(stderr, "NeuralNetwork last layer.\n");


    for(int pRow=0; pRow<10; pRow++)
    {
        outputNode[pRow] = lastHiddenLayer[pRow].output * lastHiddenLayer[pRow].lastWeight;
        outputNode[pRow] = sigmoidAdjust(outputNode[pRow]);
    }//put values in the output layer
    fprintf(stderr, "NeuralNetwork output.\n");

    for(int pRow=0; pRow<10; pRow++)
    {
        fprintf(stderr, "%lf ",outputNode[pRow]);
        fprintf(stderr, "\n");
    }

    return outputNode;
}

double NeuralNetworkTrain(const vector<CHARACTER> param[])
{
    double ep = 0.0;
    double e = 0.0;

    for (unsigned int i = 0; i < 10; ++i)
    {
        for (unsigned int j = 0; j < param[i].size(); ++j)
        {
            ep = CharacterTrain(param[i][j].ng);
            e += ep;
        }
        char r = i + '/0';
        fprintf(stderr, "Training for %c.\n", r);
    }
    return e;
}

static bool RecogPossiLessThan2(
    const pair<char, double> &a, const pair<char, double> &b)
{
    return a.second > b.second;
}

double CharacterTrain(const NG_RESULT &ib)
{
    double* runningResult = NeuralNetworkCompare(ib);

    vector<pair<char, double> > resultNeuralNetwork;
    resultNeuralNetwork.resize(10);

    for (unsigned int i = 0; i < 10; ++i)
    {
        resultNeuralNetwork[i].first = '0' + i;
        resultNeuralNetwork[i].second = runningResult[i];
    }

    sort(resultNeuralNetwork.begin(), resultNeuralNetwork.end(), RecogPossiLessThan2);

    //char resultOne = resultNeuralNetwork[0].first;
    vector<pair<char, double> > resultCorrect;
    resultCorrect.resize(10);
    for (unsigned int i = 0; i < 10; ++i)
    {
        resultCorrect[i].first = resultNeuralNetwork[i].first;
        if(i==0)
        {
            resultCorrect[i].second = 1.0;
        }
        else
        {
            resultCorrect[i].second = 0.0;
        }
    }  //initialize the correct result to be. Except the correct digit = 1, others are all 0.

    double wp[10];
    memset(wp, 0.0, sizeof(double)*10);

    double sum = 0.0;

    double outputSigma[10];
    memset(outputSigma, 0.0, sizeof(double)*10);

    for(int i=0; i<10; i++)
    {
        outputSigma[i] = (resultNeuralNetwork[i].second)*(1-resultNeuralNetwork[i].second)*(resultCorrect[i].second-resultNeuralNetwork[i].second);
        sum += pow(resultNeuralNetwork[i].second-resultCorrect[i].second,2);

        wp[i] = alpha*outputSigma[i]*lastHiddenLayer[i].output;
        //lastHiddenLayer[i].lastWeight += wp[i];
        lastHiddenLayer[i].delta = wp[i];
    }
    double ep = sum * 0.5;   //got ep
    double hiddenSigma[10][layers+1];
    memset(hiddenSigma,0.0,sizeof(double)*10*(layers+1));

    for(int pRow=0; pRow<10; pRow++)
    {
        //hiddenSigma[pRow][layers] += hiddenLayers[lastIndex][layers-1].weight[pRow] * outputSigma[pRow];
        hiddenSigma[pRow][layers] +=  lastHiddenLayer[pRow].lastWeight * outputSigma[pRow];
        hiddenSigma[pRow][layers] *= (1-lastHiddenLayer[pRow].output)*(lastHiddenLayer[pRow].output);
    }//put values in last layer

    if(layers>0)
    {
        for(int pLayer = layers-1; pLayer>=0; pLayer--)
        {
            for(int pRow=0; pRow<10; pRow++)
            {
                for(int lastIndex = 0; lastIndex<10; lastIndex++)
                {
                    hiddenLayers[lastIndex][pLayer].delta[pRow] = alpha * hiddenSigma[pRow][pLayer+1] * hiddenLayers[lastIndex][pLayer].output;
                }
            }
        }
    }

    for(int pRow=0; pRow<10; pRow++)
    {
        lastHiddenLayer[pRow].lastWeight += lastHiddenLayer[pRow].delta;
    }
    for(int pLayer=0; pLayer<layers+1; pLayer++)
    {
        for(int pRow=0; pRow< 10; pRow++)
        {
            for(int lastIndex=0; lastIndex<10; lastIndex++)
            {
                hiddenLayers[lastIndex][pLayer].weight[pRow] += hiddenLayers[lastIndex][pLayer].delta[pRow];
            }
        }
    }

    return ep;
}



