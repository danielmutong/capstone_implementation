import torchvision.transforms as transforms
import torch
from PIL import Image
import matplotlib.pyplot as plt

import sys
import os

# load image in RGB mode (png files contains additional alpha channel)
img = Image.open('img2.jpg').convert('RGB')

# set up transformation to resize the image
resize = transforms.Resize([4, 4])
img = resize(img)
to_tensor = transforms.ToTensor()

# apply transformation and convert to Pytorch tensor
tensor = to_tensor(img)

#show tensor
plt.imshow(  tensor.permute(1, 2, 0)  )

# add another dimension at the front to get NCHW shape
tensor = tensor.unsqueeze(0)

torch.set_printoptions(profile="full")


original_stdout = sys.stdout # Save a reference to the original standard output

with open('image.txt', 'w') as f:
    sys.stdout = f # Change the standard output to the file we created.
    print(tensor)
    sys.stdout = original_stdout #

with open('image.txt', 'r') as infile, open('temp.txt', 'w') as outfile:
    temp = infile.read().replace("(", "").replace(")", "").replace("[", "{").replace("]","}").replace("tensor", "")
    outfile.write(temp)

with open('temp.txt','r') as f:
    with open('input_tensor.h','w') as f2: 
        f2.write("float input_tensor[1][3][244][244] = ")
        f2.write(f.read())
        f2.write(';')
os.remove('temp.txt')


#reprinting to confirm
tensor = torch.squeeze(tensor)
plt.imshow(  tensor.permute(1, 2, 0)  )
plt.show()