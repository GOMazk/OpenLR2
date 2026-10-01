import os
import chardet

def convert_to_utf8(folder_path):
    extensions = ['.c','.cpp','.h']
    
    for root, _, files in os.walk(folder_path):
        for file in files:
            if any(file.endswith(ext) for ext in extensions):
                file_path = os.path.join(root, file)
                
                with open(file_path, 'rb') as f:
                    raw_data = f.read()
                    result = chardet.detect(raw_data)
                    encoding = result['encoding']

                if encoding in ['utf-8','UTF-8-SIG']:
                    continue
                else:
                        with open(file_path, 'r', encoding='CP932', errors='ignore') as f:
                            content = f.read()
                        with open(file_path, 'w', encoding='utf-8') as f:
                            f.write(content)
                
                print(f'Converted: {file_path}' + ' ' +encoding)

folder_path = '.\\'
convert_to_utf8(folder_path)