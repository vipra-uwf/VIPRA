# !/bin/bash
# Flags:
# -c, --compile = compile
# -r, --run = run simulation
# -o, --map = map
# -p, --peds = pedestrians
# -m, --modules = modules
# -i, --params = module params
# -h, --help = help

COMPILE=0
RUN=0
MAP=""
PEDS=""
MODULES=""
PARAMS=""


usage()
{
    echo "Usage: vipra.sh [options]"
    echo "Options:"
    echo "  -c, --compile          Compile the VIPRA project"
    echo "  -r, --run              Run the VIPRA simulation"
    echo "  -o, --map <file>       Specify the map file"
    echo "  -p, --peds <file>      Specify the pedestrians file"
    echo "  -m, --modules <file>   Specify the modules configuration file"
    echo "  -i, --params <file>    Specify the module parameters file"
    echo "  -h, --help             Display this help message"
    exit 0
}


params="$(getopt \
            -o cro:p:m:i:h \
            --long compile,run,map:,peds:,modules:,params:,help \
            -- "$@")"


if [ $? -ne 0 ]; then
    usage
fi

eval set -- "$params"

while : 
do
    case "$1" in
        -c|--compile)
            COMPILE=1
            shift
            ;;
        -r|--run)
            RUN=1
            shift
            ;;
        -o|--map)
            MAP=(${2-})
            shift 2
            ;;
        -p|--peds)
            PEDS=(${2-})
            shift 2
            ;;
        -m|--modules)
            MODULES=(${2-})
            shift 2
            ;;
        -i|--params)
            PARAMS=(${2-})
            shift 2
            ;;
        -h|--help)
            usage
            shift
            ;;
        --)
            shift
            break
            ;;
        *)
            echo "Invalid argument: $1"
            shift
            exit 1
    esac
done


echo "map_file: ${MAP}"
echo "pedestrian_file: ${PEDS}"
echo "modules_file: ${MODULES}"
echo "params_file: ${PARAMS}"
echo "compile: ${COMPILE}"


# Check if the Docker image exists. If not, build it.
IMAGE_NAME="vipra"

if docker image inspect ${IMAGE_NAME} > /dev/null 2>&1; then
    echo "Docker image ${IMAGE_NAME} already exists."
else
    echo "Docker image ${IMAGE_NAME} does not exist. Building now..."
    docker build -t ${IMAGE_NAME} --build-arg map_file=${MAP} --build-arg pedestrian_file=${PEDS} --build-arg modules_file=${MODULES} --build-arg params_file=${PARAMS} --build-arg compile=${COMPILE} .
fi


# Check and see if a Docker container already exists. If not, create it.
CONTAINER_NAME="vipra"
container_status=$( docker ps -a -f name=vipra | grep testContainer 2> /dev/null )

if [[ ! -z ${container_status} ]]; then
    echo "Container vipra exists with status: $( echo ${container_status} | awk '{ print $7 }' )"
else
    echo "Creating new container vipra"
    docker container create -it --name vipra -v ${PWD}:/VIPRA vipra bash
fi


# Check if the container is running. If not, start it.
if [[ ${container_status} != *"Running"* ]]; then
    echo "Starting existing container vipra"
    docker start vipra
fi