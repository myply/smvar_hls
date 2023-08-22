import torch
import torch.nn as nn
import numpy as np
import copy
class Slice(nn.Module):
    # Focus wh information into c-space
    def __init__(self):  # ch_in, ch_out, kernel, stride, padding, groups
        super().__init__()

    def forward(self, x):  # x(b,c,w,h) -> y(b,4c,w/2,h/2)
        return torch.cat([x[..., ::2, ::2], x[..., 1::2, ::2], x[..., ::2, 1::2], x[..., 1::2, 1::2]], 1)
def save_im2col_mat(sim_input, in_channels, out_channels, kernel_size, stride, padding):
    # print(time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time())))

    sim_input = copy.deepcopy(sim_input).cpu()
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

in_channels=12
out_channels=80
kernel_size=3
stride=2
padding=1
input=torch.rand(1,in_channels,60,80)
save_im2col_mat(input, in_channels, out_channels, kernel_size, stride, padding)
### 1*Ho*Wo*Ci
inputsave=input.permute(0,2,3,1)
inputsave=inputsave.flatten(2)
inputsave=inputsave.squeeze(dim=0)
print(inputsave.shape)
# print(inputsave)
np.savetxt('./csv/input1.csv',inputsave.numpy(),delimiter=',')


conv1=nn.Conv2d(in_channels,out_channels,kernel_size,stride,padding,bias=True)
output1=conv1.forward(input)
print(output1.shape)
outputsave1=output1.detach().permute(0,2,3,1)
print(outputsave1.shape)
np.savetxt('./csv/output1.csv',outputsave1.flatten(2).squeeze(dim=0).numpy(),delimiter=',')

weight1=conv1.weight.detach()
print(weight1.shape)
###Co*Ci*K*K  to Co*K*K*Ci
weight1=weight1.permute(0,2,3,1)
weight1=weight1.flatten(1)
print(weight1.shape)
np.savetxt('./csv/weight1.csv',weight1.numpy(),delimiter=',')
bias1=conv1.bias.detach()
np.savetxt('./csv/bias1.csv',bias1.numpy(),delimiter=',')

# in_channels=320
# out_channels=160
# kernel_size=3
# stride=2
# padding=1
# conv2=nn.Conv2d(in_channels,out_channels,kernel_size,stride,padding,bias=True)
# output2=conv2.forward(output1)
# print('output2.shape',output2.shape)
# # outputsave2=output2.detach().permute(0,2,3,1)
# # print(outputsave2.shape)
# # np.savetxt('./csv/output2.csv',outputsave2.flatten(2).squeeze(dim=0).numpy(),delimiter=',')
#
# output3=Slice().forward(output2)
# print('output3.shape',output3.shape)
#
# output4=nn.SiLU().forward(output3)
#
#
# output5=nn.MaxPool2d(kernel_size=9, stride=1, padding=4).forward(output4)
#
# output6=nn.Upsample(size=None, scale_factor=2, mode='nearest').forward(output5)
# outputsave6=output6.detach().permute(0,2,3,1)
# print(outputsave6.shape)
# np.savetxt('./csv/output6.csv',outputsave6.flatten(2).squeeze(dim=0).numpy(),delimiter=',')
#
# weight2=conv2.weight.detach()
# print(weight2.shape)
# ###Co*Ci*K*K  to Co*K*K*Ci
# weight2=weight2.permute(0,2,3,1)
# weight2=weight2.flatten(1)
# print(weight2.shape)
# np.savetxt('./csv/weight2.csv',weight2.numpy(),delimiter=',')
# bias2=conv2.bias.detach()
# np.savetxt('./csv/bias2.csv',bias2.numpy(),delimiter=',')