# !pip install conllu
# !gdown 1LuAgqwOTsRIcYysARoYMkaHGBKy3rkBm
# !gdown 1tRS2vreMs3qV6qNLHkD384L6XLB7y0_U
# !gdown 1aRrilCwIpJZowOkg_Meo7L4c9B_QjLoU
# !gdown 1yJdlUdv8cOVdnZNit4VrkPcrbd-N8FkW
# !gdown 11yci296NmMco-hnfH0Y1nWYOVLJI_pUW
# !gdown 1X--IPgjQlxQLZhGJk241dWHgGfmixEE5
# !gdown 10M530qW7v8Xwscbu4iQ28BSmGqAHkClo
# !gdown 1yXwdLzXCW9cxdFu1De7imKqiT21rFkIh
# !gdown 1gUEIp-_wPvQiQf5PhajpxXjW2sAgjqdR
# !gdown 1w3T3Ca1OPQdjekhmBzSDqhQ0c-HDJJpT

import time
import os
import sys
import json
import pickle
import random
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

# import conllu
from conllu import parse
import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import Dataset, DataLoader
from gensim.models import Word2Vec
from sklearn.metrics import classification_report, accuracy_score, recall_score, f1_score, confusion_matrix

device = "cuda" if torch.cuda.is_available() else "cpu"
sentenceLens = {}

def accuracy(model, data):
    model.eval()
    correct = 0
    total = 0
    for i, (x, y) in enumerate(DataLoader(data, batch_size=32, shuffle=True)):
        x = x.to(model.device)
        y = y.to(model.device)

        output = model(x)

        y = y.view(-1)
        output = output.view(-1, output.shape[-1])

        _, predicted = torch.max(output, 1)
        # print x as words
        # print(x)
        # print([model.train_data.index2Word[i] for i in x.view(-1).tolist()])
        # print(x.view(-1).size(0))
        # print(y.size(0))
        # exit(22)
        # only those are suitable in which x is not SOS EOS or PAD
        suitableIndex = [i for i in range(x.view(-1).size(0)) if x.view(-1)[i] != model.train_data.PADIndex]
        # test = x.view(-1)[suitableIndex].tolist()
        # print([model.train_data.index2Word[i] for i in test])
        # exit(0)
        y_masked = y[suitableIndex]
        total += y_masked.size(0)
        correct += (predicted[suitableIndex] == y_masked).sum().item()
    return correct / total