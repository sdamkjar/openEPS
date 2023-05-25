import os
import time
from lab_data_logging.data_logger import data_logger

# Function to ask user if they want to delete all the .txt and .png files in the current directory
# If no *.txt or *.png files exist in current directory exit function without asking user
def delete_files():
    # Get the current directory path
    current_directory = os.getcwd()

    # Get the list of .txt and .png files in the current directory
    files_to_delete = [file for file in os.listdir(current_directory) if file.endswith(('.txt', '.png'))]

    # If no .txt or .png files exist, exit the function
    if not files_to_delete:
        print("No .txt or .png files found in the current directory.")
        return

    # Prompt the user to confirm deletion
    response = input(f"There are {len(files_to_delete)} .txt and .png files in the current directory. Do you want to delete them? (y/n): ")

    # If the user confirms, delete the files
    if response.lower() == "y":
        for file_name in files_to_delete:
            file_path = os.path.join(current_directory, file_name)
            os.remove(file_path)
        print("Files deleted successfully.")



delete_files()

log = data_logger()
scope = log.connect('RigolDS7034')
scope.set_screenshot_path('screenshots/*')
log.add(scope,'VAVG',1)
log.add(scope,'VAVG',2)
log.add(scope,'VAVG',3)
log.add(scope,'SCREENSHOT')
log.add(time,'elapsed')
log.add(time,'current')
log.get_data()
log.get_data()
log.get_data()
log.close_file()

