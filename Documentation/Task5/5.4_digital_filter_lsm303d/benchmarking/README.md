## Execution

Execute [csv_exporter.py](sample_data/csv_exporter.py) with `sudo python3 ./csv_exporter.py` to generate a plot for accelerometer data.
The plots get saved as X.png, Y.png and Z.png. These values are the unfiltered values. The raw data
gets saved in `data.csv` . A bunch of sample data is located under the [sample data directory](sample_data/).  
In [SensorProcessor.hpp](SensorProcessor.hpp#11) select digits for float values (resolution).  
The microcontrolller provides the accelerometer data as valid JSON.  
Generation of the plots takes a little bit of time ~ 60 seconds.  
You can also use [this plot website](https://chart-studio.plotly.com/create/#/) for reading in the csv file and generating graphs.

## Requirements

The python script needs mathplotlib. For installing it run `sudo -H pip3 install -r requirements.txt` or 
`sudo -H pip install -r requirements.txt ` depending on if you installed python3 in your system.  
Also the system has to have libatlas-base-dev installed. You can install it by `sudo apt install libatlas-base-dev` .
