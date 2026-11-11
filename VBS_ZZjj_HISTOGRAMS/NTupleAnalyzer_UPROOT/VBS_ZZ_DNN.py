#############################################################################################

# This is a classification DNN code developed for VBS ZZjj LL vs TL and TT events
# But this code can be modified to serve as DNN classification code for anything

# To run this code on the terminal :
#python VBS_ZZ_DNN.py <PDF name> <Model Name>
#For eg. python NN_MergedElec_Classifier.py output.pdf mymodel.h5

#############################################################################################


#Importing the Modules
import uproot
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import awkward as ak
from sklearn.model_selection import train_test_split
from sklearn.metrics import roc_curve,auc
from tensorflow.keras import Sequential
from tensorflow.keras.layers import Dense

#Importing the OS
import os
import warnings
warnings.filterwarnings('ignore')
import sys

#Output PDF
outputname = sys.argv[1]
modelname = sys.argv[2]

from matplotlib.backends.backend_pdf import PdfPages
pp = PdfPages(outputname)

# Scaling of variables
def Scaling(X,ub,lb,columns):
    ran = np.subtract(ub,lb)
    for i in range(len(ran)):
        colname = columns[i]
        scale   = ran[i]
        Y       = X.loc[:, X.columns == colname]
        Y       = 2*(Y/scale)-1
        X.loc[:, X.columns == colname] = Y



#Importing the root files using uproot
file_zzjjew_TT = uproot.open("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/minitree_linnuo/EWK_polar/merge/mc23_13p6TeV.546887.MGPy8EG_A14_NNPDF23LO_ZZjjEW_4l_TT_rev.merged.root")
file_zzjjew_LL = uproot.open("/home/ss1584/ATLAS_ANALYSIS/VBS_ZZjj_HISTOGRAMS/ROOTFILES/minitree_linnuo/EWK_polar/merge/mc23_13p6TeV.546889.MGPy8EG_A14_NNPDF23LO_ZZjjEW_4l_LL_rev.merged.root")

#Extracting the trees from the root files
tree_name = "Nominal;1"
tree_ZZjjEW_TT = file_zzjjew_TT[tree_name]
tree_ZZjjEW_LL = file_zzjjew_LL[tree_name]


#Extracting the branches and creating the datframe
branches_ZZjjEW_TT = tree_ZZjjEW_TT.keys()
branches_ZZjjEW_LL = tree_ZZjjEW_LL.keys()


awk_ZZjjEW_TT = tree_ZZjjEW_TT.arrays(branches_ZZjjEW_TT)
awk_ZZjjEW_LL = tree_ZZjjEW_LL.arrays(branches_ZZjjEW_LL)
df_zzjjew_TT = pd.DataFrame(awk_ZZjjEW_TT.to_list())
df_zzjjew_LL = pd.DataFrame(awk_ZZjjEW_LL.to_list())


#Declaring some branches from Tree, many of which will be later used for training. Right now these branches will be helpful in exploding the dataframe.
listofvars = ['mllll','pT_llll']

df_zzjjew_TT = df_zzjjew_TT[listofvars]
df_zzjjew_LL = df_zzjjew_LL[listofvars]


#exploding the dataframe so that we look at object level and not event level
df_zzjjew_TT = df_zzjjew_TT[listofvars].apply(lambda col: col.explode()).reset_index(drop=True)
df_zzjjew_LL = df_zzjjew_LL[listofvars].apply(lambda col: col.explode()).reset_index(drop=True)

print(df_zzjjew_TT)


trainingvars = listofvars
#Giving Labels to the Signal-0 and Background-1 (Signal - J/Psi and Bkg - Z)
df_zzjjew_LL['label']=1
df_zzjjew_TT['label']=0
        
 
data = pd.concat([df_zzjjew_TT,df_zzjjew_LL])

X = data[listofvars]
y = data['label']

#Splitting the data into training and testing datasets
X_train,X_test,y_train,y_test = train_test_split(X,y,test_size=0.5)


X_train = X_train[trainingvars]
X_test  = X_test[trainingvars]

n_features = X_train.shape[1] 

#Defining the Model
model = Sequential()
model.add(Dense(256, activation='relu', kernel_initializer='he_normal',input_dim=n_features))
model.add(Dense(128, activation='relu', kernel_initializer='he_normal'))
model.add(Dense(64, activation='relu', kernel_initializer='he_normal'))
model.add(Dense(16, activation='relu', kernel_initializer='he_normal'))
model.add(Dense(1, activation='sigmoid'))

#Compiling the model
model.compile(optimizer='adam', loss='binary_crossentropy', metrics=['accuracy'])

#Converting all objects to tensorflow objects (for training)
import tensorflow as tf

X_train = tf.convert_to_tensor(X_train, dtype=tf.float32)
X_test = tf.convert_to_tensor(X_test, dtype=tf.float32)
y_train = tf.convert_to_tensor(y_train, dtype=tf.float32)
y_test = tf.convert_to_tensor(y_test, dtype=tf.float32)
        
#Training the model
history = model.fit(X_train,y_train,epochs=50,batch_size=512,validation_data=(X_test,y_test),verbose=1)       
        
model.summary()
model.save('Models/'+modelname)   
#Plotting the Loss function
plt.figure(figsize=(7,5))
plt.plot(history.history['loss'], label='Train Loss')
plt.plot(history.history['val_loss'], label = 'Val Loss')
plt.xlabel('Epoch')
plt.ylabel('Loss')
#plt.ylim([0, 10])
#plt.yscale('log')
plt.legend(loc='upper right')
#plt.savefig('loss_v_epoch.png')
plt.savefig(pp, format='pdf')
plt.close()   

#predicted value of y for training and testing from the model
y_test_pred = model.predict(X_test)
y_train_pred = model.predict(X_train)

#getting the label value of signal(JPsi i.e 1) and background(Z i.e 0) for test data
true_val=1
y_test_pred_signal = [y_test_pred[i] for i in range(len(y_test)) if y_test[i]==true_val]

true_val=0
y_test_pred_bkg = [y_test_pred[i] for i in range(len(y_test)) if y_test[i]==true_val]

#getting the label value of signal(JPsi i.e 1) and background(Z i.e 0) for training data
true_val=1
y_train_pred_signal = [y_train_pred[i] for i in range(len(y_train)) if y_train[i]==true_val]

true_val=0
y_train_pred_bkg = [y_train_pred[i] for i in range(len(y_train)) if y_train[i]==true_val]

#Plotting the NN scores for testing and training data
plt.hist(np.array(y_test_pred_signal),label='test score sig',color='red',alpha=0.7,bins=np.arange(0,1,0.01),histtype='stepfilled',weights=np.ones_like(np.array(y_test_pred_signal))/len(np.array(y_test_pred_signal)))
plt.hist(np.array(y_test_pred_bkg),label='test score bkg',color='green',alpha=0.7,bins=np.arange(0,1,0.01),histtype='stepfilled',weights=np.ones_like(np.array(y_test_pred_bkg))/len(np.array(y_test_pred_bkg)))

plt.hist(np.array(y_train_pred_signal),label='train score sig',color='red',linewidth=2,bins=np.arange(0,1,0.01),histtype='step',weights=np.ones_like(np.array(y_train_pred_signal))/len(np.array(y_train_pred_signal)))
plt.hist(np.array(y_train_pred_bkg),label='train score bkg',color='green',linewidth=2,bins=np.arange(0,1,0.01),histtype='step',weights=np.ones_like(np.array(y_train_pred_bkg))/len(np.array(y_train_pred_bkg)))
plt.xlabel('Predicted Score')
plt.ylabel('Frequency')
plt.grid(True)
plt.legend()
plt.savefig(pp, format='pdf')
plt.close()

#ROC curve for testing
fpr,tpr,_ = roc_curve(y_test,y_test_pred)
auc_score_test = auc(fpr,tpr)

#ROC curve for Training

fpr1,tpr1,_ = roc_curve(y_train,y_train_pred)
auc_score_train = auc(fpr1,tpr1)

#Plotting the ROC
plt.figure(figsize=(8,8))
plt.plot(fpr,tpr,color='blue', label='Testing ROC (AUC = %0.4f)' % auc_score_test)
plt.plot(fpr1,tpr1,color='red', label='Training ROC (AUC = %0.4f)' % auc_score_train)
plt.legend(loc='lower right')
plt.title(f'ROC Curve',fontsize=20)
plt.xlabel('False Positive Rate',fontsize=20)
plt.ylabel('True Positive Rate',fontsize=20)
plt.xlim(0.,1.)
plt.ylim(0.,1.)
plt.show()
plt.savefig(pp, format='pdf')
plt.close()

pp.close()

print(f'All done. Model is saved as {modelname}')

