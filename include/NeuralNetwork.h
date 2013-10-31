#pragma once
#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <NineGrids.h>
#include <Node.h>
#include <LastNode.h>
#include <vector>
#include <Characteristic.h>

using namespace std;

double* NeuralNetworkCompare(const NG_RESULT &ia);
double NeuralNetworkTrain(const vector<CHARACTER> param[]);
double CharacterTrain(const NG_RESULT &ib);
static const unsigned int layers = 2;  //layers in the middle, >= 0

#endif // NEURALNETWORK_H
