# TP-WIK-DPS-TP01

# build docker image one staging

```
docker build -t tp02 .
```

# build docjer image multi staging

```
docker build -t tp02-ms -f dockerfile-ms .
```

# Run image
```
docker run -it -p "8080:8080" -t <image-name> 
```

