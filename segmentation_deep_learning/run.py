from main import Main
from models.unet import Unet
from loss_functions import LossFunctions

loss_obj = LossFunctions()

### Experiment 1 START ###
main_obj = Main()

experiment_name = 'exp1'
main_obj.experiment_path = 'results/' + experiment_name + '/'

# training parameters
main_obj.epochs = 1
main_obj.batch_size = 4
main_obj.validation_split = 0.3

# data, masks
main_obj.train_file_path = '/content/drive/datasets/dataset'
main_obj.test_file_path = '/content/drive/datasets/images'

# compile parameters
main_obj.learning_rate = 0.001
main_obj.loss = loss_obj.dice_loss

# Train, Test, Eval
model_obj = Unet()
main_obj.train(model_obj, load_model=0, model_path='', name=experiment_name)
main_obj.test(model_path=main_obj.experiment_path + 'best_weights.ckpt', evaluation=True)
### Experiment 1 END ###


