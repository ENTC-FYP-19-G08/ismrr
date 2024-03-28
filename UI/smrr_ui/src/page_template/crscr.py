Name=input("Enter Screen Name: ")
name=Name.lower()
NAME=Name.upper()

template_paths=["sceen_${name}.cpp","sceen_${name}.h","sceen_${name}.ui"]
new_file_paths=["../src/sceen_${name}.cpp","../include/sceen_${name}.h","../ui/sceen_${name}.ui"]


for template_path,new_file_path in zip(template_paths,new_file_paths):
    with open(template_path,'r') as template_file:
        template_content=template_file.read()
        new_file_content=template_content.replace("${Name}",Name).replace("${name}",name).replace("${NAME}",NAME)
        with open(new_file_path.replace("${name}",name),'w') as new_file:
            new_file.write(new_file_content)
        


