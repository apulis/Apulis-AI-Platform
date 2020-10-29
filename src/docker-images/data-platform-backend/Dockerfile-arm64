FROM apulistech/dotnetcore-sdk:3.1-arm64 AS build-env
RUN git clone --branch arm https://c754fb803c3f5c7fe91780f1a7222275bea3cbbf@github.com/apulis/data-platform-backend.git /RemoteSensing-UI
WORKDIR /RemoteSensing-UI/src/projects/WebUI/WebUI
RUN dotnet publish --configuration Release

FROM apulistech/dotnetcore-runtime:3.1-arm64
WORKDIR /app
COPY --from=build-env /RemoteSensing-UI/src/projects/WebUI/WebUI/bin/Release/netcoreapp3.1/publish .