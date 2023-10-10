#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Usage: $0 <directory_path>"
  exit -1
fi

directory_path="$1"

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

files_str=$(list_zero_size_files "$directory_path")
declare -a files_arr=()
while IFS= read -r line; do
  files_arr+=("$line")
done <<< "$files_str"

# Size of the list
size="${#files_arr[@]}"
if [ "$size" == 0 ]; then
  echo "No empty files found. Exit."
  exit 0
fi

for ((i=1;i<="$size";i++)); do
  ./menu $files_str 
  chosen_file=$?
  ((chosen_file -= 1))

  file="${files_arr[$chosen_file]}"
  echo "Do you want to delete this file: $file"

  ./menu Yes No
  decision=$?

  if [ "$decision" == 1 ]; then
    delete_file "$file"
  fi
done
