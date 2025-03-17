
import sys
import os
import json

def find_badl_components(root_dir):
    """Recursively find all 'badl.component' files in subdirectories."""
    components = []
    stimuli = []
    for dirpath, _, filenames in os.walk(root_dir):
        if 'badl.component' in filenames:
            file_path = os.path.join(dirpath, 'badl.component')
            components.append(file_path)
        if 'badl.stimulus' in filenames:
            file_path = os.path.join(dirpath, 'badl.stimulus')
            stimuli.append(file_path)
    return components, stimuli

def read_component_info(file_path):
    """Extract Type and Name from a 'badl.component' file."""
    component_info = {}
    try:
        component_info['directory'] = os.path.dirname(file_path)
        with open(file_path, 'r', encoding='utf-8') as file:
            for line in file:
                if line.startswith("Type:"):
                    component_info["Type"] = line.split("Type:", 1)[1].strip()
                elif line.startswith("Include:"):
                    component_info["Include"] = line.split("Include:", 1)[1].strip()
                elif line.startswith("Name:"):
                    component_info["Name"] = line.split("Name:", 1)[1].strip()
    except Exception as e:
        print(f"Error reading {file_path}: {e}")
    return component_info

def categorize_components(components):
    """Organize components into a categorized dictionary."""
    categorized = {
        "Sensor": [],
        "InternalState": [],
        "Actuator": [],
        "Attention": [],
        "ThoughtProcess": []
    }
    for component in components:
        comp_type = component.get("Type")
        categorized[comp_type].append(component)
    return categorized

def apply_component_mappings(components, mappings):
    """Apply name mappings from JSON to each component based on Type."""
    updated_components = []
    for component in components:
        comp_type = component.get("Type")
        if comp_type and comp_type in mappings:
            component["MappedName"] = mappings[comp_type]
        updated_components.append(component)
    return updated_components

def check_components(agent_components, components):
    for key in components:
        for agentComp in agent_components[key]:
            if not any(obj['Name'] == agentComp for obj in components[key]):
                raise Exception(f'{agentComp} Not in available components: {components[key]}')
            
def get_includes(agent_components, components, stimuli):
    includes = []
    for key in agent_components:
        if key != 'ProgramInterface':
            for agentComp in agent_components[key]:
                if key == 'Stimuli':
                    for stimulus in stimuli:
                        if stimulus['Name'] == agentComp:
                            includes.append(f"{stimulus['directory']}/{stimulus['Include']}")
                else:
                    for comp in components[key]:
                        if comp['Name'] == agentComp:
                           includes.append(f"{comp['directory']}/{comp['Include']}")

    return includes

def replace_placeholders(template_file, output_file, agent_components, components, stimuli):
    """Replace placeholders in the template file with component values."""

    check_components(agent_components, components)

    replacements = {
        "@BADL_STIMULI_TYPES@": ", ".join(agent_components['Stimuli']),
        "@BADL_AGENT_THOUGHT_PROCESS_COMPONENTS@": ", ".join(agent_components['ThoughtProcess']),
        "@BADL_AGENT_ACTUATOR_COMPONENTS@": ", ".join(agent_components['Actuator']),
        "@BADL_AGENT_ATTENTION_COMPONENTS@": ", ".join(agent_components['Attention']),
        "@BADL_AGENT_INTERNAL_STATE_COMPONENTS@": ", ".join(agent_components['InternalState']),
        "@BADL_AGENT_SENSOR_COMPONENTS@": ", ".join(agent_components['Sensor']),
    }

    try:
        with open(template_file, 'r', encoding='utf-8') as file:
            content = file.read()
        
        for placeholder, value in replacements.items():
            content = content.replace(placeholder, value)
        
        with open(output_file, 'w', encoding='utf-8') as file:
            file.write(content)


        with open(f'{os.path.dirname(output_file)}/program_interface.hpp.in', 'r', encoding='utf-8') as file:
            content = file.read()
        
        with open(f'{os.path.dirname(output_file)}/program_interface.hpp', 'w', encoding='utf-8') as file:
            file.write(content.replace("@BADL_PROGRAM_INTERFACE_INCLUDE@", f"\"{agent_components['ProgramInterface']}\""))

        with open(f'{os.path.dirname(output_file)}/build_includes.hpp', 'w') as file:
            file.write("\n".join(f"#include \"{include}\"" for include in get_includes(agent_components, components, stimuli)))

        with open(f'{os.path.dirname(output_file)}/build_includes.hpp', 'w') as file:
            file.write("\n".join(f"#include \"{include}\"" for include in get_includes(agent_components, components, stimuli)))
            
    except Exception as e:
        print(f"Error processing template file: {e}")

def main():
    badl_directory = sys.argv[1]
    root_directory = sys.argv[2]
    agent_file = sys.argv[3]

    print(sys.argv[2], sys.argv[3])
    
    # Load JSON mappings
    try:
        with open(agent_file, 'r', encoding='utf-8') as f:
            agent_components = json.load(f)
    except Exception as e:
        print(f"Error loading JSON file: {e}")
        return
    
    # Find components
    component_files, stimuli_files = find_badl_components(root_directory)
    
    # Extract information
    components = [read_component_info(file) for file in component_files]
    stimuli = [read_component_info(file) for file in component_files]
    
    # Categorize components
    categorized_components = categorize_components(components)
    
    # Replace placeholders in template file
    replace_placeholders(f'{badl_directory}/badl/build_macros.hpp.in', f'{badl_directory}/badl/build_macros.hpp', agent_components, categorized_components, stimuli)
    
    print("Template processing complete. Output saved to", f'{badl_directory}/badl/build_macros.hpp')

if __name__ == "__main__":
    main()