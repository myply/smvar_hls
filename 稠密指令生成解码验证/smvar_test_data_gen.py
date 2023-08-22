import torch
import torch.nn as nn
import numpy as np
def save_im2col_mat(sim_input, in_channels, out_channels, kernel_size, stride, padding):
    # print(time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time())))

    sim_input = sim_input.cpu()
    p_input = torch.nn.ZeroPad2d(padding).forward(sim_input)
    # print("p_input",p_input.shape)

    # im2col for weight*input
    kernel_area_size = kernel_size * kernel_size
    # print('p_input_row', p_input.shape[2] )
    # output_row = int(round((p_input.shape[2] - kernel_size) / stride + 1))
    # output_col = int(round((p_input.shape[3] - kernel_size) / stride + 1))
    output_row = int(int((p_input.shape[2] - kernel_size) / stride + 1))
    output_col = int(int((p_input.shape[3] - kernel_size) / stride + 1))
    print('output_row',output_row)
    print('output_col', output_col)
    output_area_size = output_row * output_col
    # mul_weight matrix need transposeprint


    mul_input = torch.rand(output_area_size, in_channels,kernel_area_size)


    # get mul_input
    for i in range(output_row):
        for j in range(output_col):
            for k in range(in_channels):
                # print('shape1',mul_input[i*output_col+j][k*kernel_area_size:(k+1)*kernel_area_size].shape)
                # print('shape2',p_input[0, k, i * stride:i * stride + kernel_size,j * stride:j * stride + kernel_size].flatten().shape)
                # mul_input[i*output_col+j][k*kernel_area_size:(k+1)*kernel_area_size]=p_input[0, k,i*stride:i*stride+kernel_size,j*stride:j*stride+kernel_size].flatten()
                mul_input[i * output_col + j][k :(k + 1)] = p_input[0, k, i * stride:i * stride + kernel_size,
                                                                                                 j * stride:j * stride + kernel_size].flatten()
    ###（Ho*Wo）*(K*K*Ci)
    mul_input=mul_input.permute(0,2,1).flatten(1)
    print("im2col mat shape")
    print(mul_input.shape)
    np.savetxt('./csv/input_im2col.csv', mul_input.numpy(), delimiter=',')

in_channels=160
out_channels=320
kernel_size=3
stride=2
padding=1
input=torch.rand(1,in_channels,240,320)
print("")
### 1*Ho*Wo*Ci
inputsave=input.permute(0,2,3,1)
inputsave=inputsave.flatten(2)
inputsave=inputsave.squeeze(dim=0)
print(inputsave.shape)
print('inputsave')
# print(inputsave)
print(inputsave[0][0:32])
np.savetxt('./csv/input.csv',inputsave.numpy(),delimiter=',')

print("input:")
print(input)
conv=nn.Conv2d(in_channels,out_channels,kernel_size,stride,padding,bias=False)
output=conv.forward(input)
outputsave=output.detach().permute(0,2,3,1)
print(outputsave.shape)
print('output',outputsave)
np.savetxt('./csv/output.csv',outputsave.flatten(2).squeeze(dim=0).numpy(),delimiter=',')


weight=conv.weight.detach()
print(weight.shape)
###Co*Ci*K*K  to Co*K*K*Ci
weight=weight.permute(0,2,3,1)
weight=weight.flatten(1)
print(weight.shape)
np.savetxt('./csv/weight.csv',weight.numpy(),delimiter=',')

save_im2col_mat(input, in_channels, out_channels, kernel_size, stride, padding)