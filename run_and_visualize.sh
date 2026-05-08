# !/bin/bash

if [[ "$1" == "c" ]] || [[ "$2" == "c" ]] || [[ "$3" == "c" ]]; then
echo "Compiling"
# cmake --build build --target vipra -- -j$(nproc)
sudo make
# sudo make debug
echo "Compilation Complete"
fi

if [[ "$1" == "r" ]] || [[ "$2" == "r" ]] || [[ "$3" == "r" ]]; then
echo "Starting Sim"
# ./vipra -map=temp/museum_quads.dxf -peds=maps/pedestrians/a320_pedestrians.json -params=temp/module_params_no_file.json -modules=temp/sim_config_potential-field.json
./vipra -map=temp/museum_quads.dxf -peds=maps/pedestrians/a320_pedestrians.json -params=temp/module_params_no_file.json -modules=temp/sim_config_json.json
# ./VIPRA -map=temp/museum.dxf -peds=maps/pedestrians/a320_pedestrians.json -params=temp/module_params_no_file.json
# ./VIPRA -map=temp/boeing_727.dxf -peds=temp/boeing_727_pedestrians.json -params=temp/module_params.json -modules=temp/sim_config.json
# ./VIPRA -map=temp/closing_geometry_test.dxf -peds=maps/pedestrians/boeing_727_pedestrians.json -params=temp/module_params.json -modules=temp/sim_config.json
echo "Sim complete"
fi

if [[ "$1" == "v" ]] || [[ "$2" == "v" ]] || [[ "$3" == "v" ]]; then
echo "Starting Visualizer"
python3 tools/trajectories_visualizer/visualize.py -peds sim_output/0/trajectories.json -obs temp/museum_quads.dxf -outpath museum_test.mp4 -xDim -5,65 -yDim 3,80
# python3 tools/trajectories_visualizer/visualize.py -peds sim_output/0/trajectories.json -obs temp/boeing_727.dxf -outpath airline_727.mp4 -xDim 3,37 -yDim 0,10
# python3 tools/trajectories_visualizer/visualize.py -peds sim_output/0/trajectories.json -obs temp/closing_geometry_test.dxf -outpath small_test.mp4 -xDim 0,40 -yDim 3,30
echo "Visualization Complete"
fi
