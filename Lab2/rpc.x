const MAXNAME = 255;

struct file_data {
    string filename<MAXNAME>;
    opaque data<>;
};

program FILETRANSFER {
    version FILETRANSFER_V1 {
        file_data DOWNLOAD_FILE(string) = 1;
    } = 1;
} = 0x31230000;