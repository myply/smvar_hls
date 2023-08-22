import torch
import torch.nn as nn
import numpy as np
input=torch.rand(1,32,96,160)
inputsave=input.permute(0,2,3,1)
inputsave=inputsave.flatten(2)
inputsave=inputsave.squeeze(dim=0)
print(inputsave.shape)
np.savetxt('./csv/input.csv',inputsave.numpy(),delimiter=',')


conv=nn.Conv2d(in_channels=32,out_channels=64,kernel_size=3,stride=2,padding=1,bias=False)
output=conv.forward(input)
weight=conv.weight.detach()
print(weight.shape)
###Co*Ci*K*K  to Co*K*K*Ci
weight=weight.permute(0,2,3,1)
weight=weight.flatten(1)
print(weight.shape)
np.savetxt('./csv/weight.csv',weight.numpy(),delimiter=',')