#!/bin/bash

# NOTICE: The executable file './menu' must be in the same directory as this file #

# Check if a directory path is provided as an argument
if [ $# -ne 1 ]; then
  echo "Usage: $0 <directory_path>"
  exit -1
fi

# Directory path
directory_path="$1"

# Check if the provided path is a directory
if [ ! -d "$directory_path" ]; then
  echo "Error: '$directory_path' is not a directory."
  exit -1
fi

############################################################

# Function to list empty files recursively
list_zero_size_files() {
  local dir="$1"
  find "$dir" -type f -size 0
}

# Function to delete a file
delete_file() {
  local file_path="$1"
  rm -f "$file_path"
  echo "File '$file_path' has been deleted."
}

############################################################

# List empty files in string
zero_size_files_str=$(list_zero_size_files "$directory_path")

# List empty files in array of string
declare -a zero_size_files_arr=()
while IFS= read -r line; do
  zero_size_files_arr+=("$line")
done <<< "$zero_size_files_str"

# Size of the list
size="${#zero_size_files_arr[@]}"
if [ "$size" == 0 ]; then
  echo "No empty files found. Exit."
  exit 0
fi

for ((i=1;i<="$size";i++)); do
  ./menu $zero_size_files_str 
  # The file index in array
  chosen_file=$?
  ((chosen_file -= 1))

  # If the input is exit, exit the program
  if [ "$chosen_file" -lt 0 ]; then
    exit -1
  fi

  file="${zero_size_files_arr[$chosen_file]}"
  echo "Do you want to delete this file: $file"

  ./menu Yes No
  decision=$?

  # If the input is YES, delete the file
  if [ "$decision" == 1 ]; then
    delete_file "$file"
  fi
done
