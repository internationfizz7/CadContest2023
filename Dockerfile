FROM --platform=linux/amd64 ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libstdc++6 \
    libgcc-s1 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

COPY NCKUFplanner .
RUN chmod +x NCKUFplanner

CMD ["bash"]
