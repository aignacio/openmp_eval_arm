docker run --rm --name arm_gcc_test -it -v $(pwd):/src -w /src arm_gcc_aignacio bash
docker build -t arm_gcc_aignacio:latest .
