Protocol Analysis 
Step 1. Install the software:
- SigRok is the opensource software that is more hardware universal but does not have an Apple ARM version 
    - Windows Insataller: 
        - Download installer from https://sigrok.org/wiki/Downloads
    - Linux Instal:
        - Install via package manager: 
        '''
            sudo apt install pulseview sigrok
            Add udev rules for device access: 
            sudo cp 60-libsigrok.rules /etc/udev/rules.d/
            sudo udevadm control --reload-rules
        '''
    - Launch the app via terminal by running, pulseview  
    - Once installed access the Pulseview app

- Saleae Logic 2 Software supports all OS downoads: 
    - Download installer from https://www.saleae.com/pages/downloads 
    - Once installed access the Saleae app

Step 2. Setup your Logic Analyzer 
- SALEAE
    - Plug in your Logic Analyzer to your computer 
    - Saleae Logic 2 Software should detect the device
    - Software will open a session, if it does not, create a new session 
- Sigrok:
    - Click ‘Demo Device’ in the top bar
    - Choose the Driver
    -   Choose "fx2lafw (generic driver for FX2 based LAS) (fx2lafw)"
    - Choose Interface: USB
    - Click “Scan for devices using driver above” button
    - You should see “DEVICE NAME” and select it


Step 3. Data Collection
- SALEAE:
    - On the right side panel choose the 'Analyzer' button
    - Press on the '+' of the Analyzer Panel, select 'CAN'
    - In the CAN popup 
        - Choose your Channel
        - Set the Bit Rate to 250000 
    - Begin Capture
    - End Capture when enough data has been collected
- Sigrok: 
    - In the top bar select the following sample rates:
        - 1M Samples 
        - 250 kHz 
    - Press the Run button found on the top left side of the screen 
    - Allow it to run, until all samples are taken or when the run has gone for the time of your choosing


Step 4. Analyze Raw Data
- Access this page [CAN OVERVIEW:](https://github.com/thedini/openBridge/blob/main/docs/can_protocol.md) for introduction to CAN
- Work through header breakdown
- Observe stuffed bits

Step 5. Export Data
- SALEAE:
    - Raw Data Export:
        - File -> Export Raw Data
        - This Export allows for the raw digital data that we can use for further binary analysis
    - Capture File Export: (optional)
        - File -> Save Capture 
        - This export allows for saving the particular capture
- Sigrok:
    - Raw Data Export:
        - Save -> Export Raw Binary logic data
        - This Export allows for the raw digital data that we can use for further binary analysis
    - Capture File Export: (optional)
        - Save -> Save Capture 
        - This export allows for saving the particular capture


Step 6. Converting Raw Data into Hexadecimal format
- To be continued... time permitting

