from pathlib import Path
import deeplabcut

model_path = Path(deeplabcut.utils.auxiliaryfunctions.get_deeplabcut_path())
models = [
    "resnet_50",
    "resnet_101",
    "resnet_152",
    "mobilenet_v2_0.35",
    "mobilenet_v2_0.5",
    "mobilenet_v2_0.75",
    "mobilenet_v2_1",
    "efficientnet-b0",
    "efficientnet-b1",
    "efficientnet-b2",
    "efficientnet-b3",
    "efficientnet-b4",
    "efficientnet-b5",
    "efficientnet-b6",
]
for model in models:
    deeplabcut.utils.auxfun_models.Check4Weights(model, model_path)
