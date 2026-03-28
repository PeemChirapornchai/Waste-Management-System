from ultralytics import YOLO

# Load the model you downloaded from GitHub
model = YOLO("./detection/models/last.pt")

# Print the dictionary of classes it knows
print(model.names)