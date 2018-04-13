clear all;
close all;
clc;
fid = fopen('stream.txt','r');
datastream = fscanf(fid,'%d');
datastream = datastream';
fclose(fid);

s = 10^5; %the size of the sample set
sample_set = datastream(1:s);
for index=s+1:1:length(datastream)
    if rand<(s/index)
        discard_element_index = ceil(rand*s);
        sample_set(discard_element_index) = datastream(index);
    end
end
sum(sample_set)/length(sample_set)
sum(datastream)/length(datastream)