# !/bin/bash

# TODO(tylerk): Should there even be an option to compile? Or should this be done by default?
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
    echo "  -o, --map <file>       Specify the map file (Required for run)"
    echo "  -p, --peds <file>      Specify the pedestrians file (Required for run)"
    echo "  -m, --modules <file>   Specify the modules configuration file (Required for run)"
    echo "  -i, --params <file>    Specify the module parameters file (Required for run)"
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

# Check if the Docker image exists. If not, build it.
IMAGE_NAME="vipra"

if docker image inspect ${IMAGE_NAME} > /dev/null 2>&1; then
    echo "Docker image ${IMAGE_NAME} already exists."
else
    echo "Docker image ${IMAGE_NAME} does not exist. Building now..."
    docker build -t ${IMAGE_NAME} --build-arg compile=${COMPILE} .
fi


# Check and see if a Docker container already exists. If not, create it.
CONTAINER_NAME="vipra"
container_status=$( docker ps -a --format "table {{.Status}}\t{{.Names}}" -f name=vipra | grep vipra 2> /dev/null )

if [[ ! -z ${container_status} ]]; then
    echo "Container vipra exists with status: $( echo ${container_status} | awk '{ print $1 }' )"
else
    echo "Creating new container vipra"
    docker container create -it --name vipra -v ${PWD}:/VIPRA vipra bash
    # docker container create -it --name vipra vipra bash
fi


# Check if the container is running. If not, start it.
if [[ ${container_status,,} != *"up"* ]]; then
    echo "Starting existing container vipra"
    docker start vipra
fi

# Run VIPRA inside the container.
if [[ ${RUN} -eq 1 ]]; then

    if [[ -z "${MAP}" || -z "${PEDS}" || -z "${MODULES}" || -z "${PARAMS}" ]]; then
        echo "Error: To run the simulation, you must specify the map, pedestrians, modules, and params files."
        usage
        exit 1
    fi

    docker exec -it vipra bash -c "/usr/local/bin/vipra -map=${MAP} -peds=${PEDS} -params=./${PARAMS} -modules=./${MODULES}"
fi
