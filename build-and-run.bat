@echo off
setlocal enabledelayedexpansion

if "%~1"=="" (
  echo Usage: %~nx0 ^<input_points_file^> ^<planes_file^>
  exit /b 1
)

if "%~2"=="" (
  echo Usage: %~nx0 ^<input_points_file^> ^<planes_file^>
  exit /b 1
)

set INPUT_FILE=%~1
set PLANES_FILE=%~2
set BUILD_DIR=build
set VENV_DIR=myenv

echo Building the project...

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%
cmake .. 
if errorlevel 1 (
  echo CMake configuration failed.
  exit /b 1
)
cmake --build . 
if errorlevel 1 (
  echo Build failed.
  exit /b 1
)
cd ..

echo Setting up Python virtual environment...

if not exist %VENV_DIR% (
  python -m venv %VENV_DIR%
)

call %VENV_DIR%\Scripts\activate.bat
if errorlevel 1 (
  echo Failed to activate virtual environment.
  exit /b 1
)

pip install --upgrade pip
pip install -r requirements.txt

echo Running filter on %INPUT_FILE% with planes file %PLANES_FILE%...

%BUILD_DIR%\Debug\filter.exe %INPUT_FILE% %PLANES_FILE%
if errorlevel 1 (
  echo Filter execution failed.
  exit /b 1
)

rem Get base filename without extension
for %%f in ("%INPUT_FILE%") do set BASENAME=%%~nf
set GOOD_POINTS=%BASENAME%_good.txt

echo Running visualization on %GOOD_POINTS% ...

python visualize.py %GOOD_POINTS%

endlocal
