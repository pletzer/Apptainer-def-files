#!/usr/bin/env python3

import os
import shutil
from pathlib import Path

def copy_def_files(source_dir, target_dir):
    """
    Scan directories for .def files and copy them along with their parent directory
    to the target location.
    
    Args:
        source_dir (str): Source directory path containing container directories
        target_dir (str): Target directory path where .def files will be copied
    """
    # Convert paths to Path objects for better handling
    source_path = Path(source_dir)
    target_path = Path(target_dir)
    
    # Create target directory if it doesn't exist
    target_path.mkdir(parents=True, exist_ok=True)
    
    # Counter for statistics
    dirs_processed = 0
    def_files_found = 0
    
    print(f"Scanning {source_dir} for .def files...")
    
    # Iterate through immediate subdirectories
    for item in source_path.iterdir():
        if item.is_dir():
            dirs_processed += 1
            def_files = list(item.glob('*.def'))
            
            if def_files:
                def_files_found += len(def_files)
                dir_name = item.name
                new_dir = target_path / dir_name
                
                # Create the directory in the target location
                new_dir.mkdir(parents=True, exist_ok=True)
                
                # Copy only .def files
                for def_file in def_files:
                    print(f"Copying {def_file} to {new_dir}")
                    shutil.copy2(def_file, new_dir)
    
    # Print summary
    print("\nSummary:")
    print(f"Directories processed: {dirs_processed}")
    print(f"Directories with .def files: {def_files_found}")

if __name__ == "__main__":
    source_directory = "/opt/nesi/containers"
    target_directory = "/nesi/project/nesi99999/Dinindu/Apptainer-def-files"
    
    try:
        copy_def_files(source_directory, target_directory)
        print("\nOperation completed successfully!")
    except Exception as e:
        print(f"\nAn error occurred: {e}")
