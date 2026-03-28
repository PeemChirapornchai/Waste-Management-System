import requests

# The URL of your Django server running in Docker
url = 'http://127.0.0.1:8000/api/waste/'

# Open the dummy image file
file_path = 'wasteTest.jpg'

try:
    with open(file_path, 'rb') as img:
        # Package the image exactly how the ESP32 will do it
        files = {'image': img}
        
        print("Sending image to server...")
        # Send the POST request
        response = requests.post(url, files=files)
        
        # Print the server's reply!
        print("Server Reply:", response.json())

except FileNotFoundError:
    print(f"Error: Could not find '{file_path}'. Please add a test image to the folder.")