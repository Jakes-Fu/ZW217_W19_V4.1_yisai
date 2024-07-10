#!/bin/bash


files=(
  "./w217-release-V0.0.1(base-w22.47.7).7z"
  "./w217-release-V0.0.1-patch.rar"
  "./w217-release-V0.0.2-patch.zip"
)
folders=(
  "./build"
  "./Third-party/oneos/bootloader_src"
  "./Third-party/oneos/ota/cmiot_src"
  "./Third-party/oneos/cms/cms_core"
)


for file in "${files[@]}"; do
  if [ -f "$file" ]; then
    rm "$file"
    echo "remove $file success！"
  else
    echo "file $file not exist"
  fi
done


for folder in "${folders[@]}"; do
  if [ -d "$folder" ]; then
    rm -rf "$folder"
    echo "folder $folder clean！"
  else
    echo "floder $folder not exist"
  fi
done
read -rp "press Enter to exit"
