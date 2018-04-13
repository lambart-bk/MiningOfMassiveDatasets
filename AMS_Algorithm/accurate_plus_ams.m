clear all;
close all;
clc;
fid = fopen('stream_sample_ams.txt','r');
datastream = fscanf(fid,'%d');
datastream = datastream';
fclose(fid);

%datastream = [1,2,3,2,4,1,3,4,1,2,4,3,1,1,2];

unique_set = unique(datastream); %counter_set1 = hist(datastream,unique(datastream));
counter_set = zeros(1,length(unique_set));
for i=1:length(counter_set)
    key = unique_set(i);
    counter_set(i) = length(find(datastream==key));
end  

sec_moment = 0;
for i=1:length(counter_set)
    sec_moment = sec_moment + counter_set(i)^2;
end

err_sum = 0;
for iter = 1:20
group_num = 6; %total groups
estimate_num = 100; %num of element in one group
pos_matrix = rand(group_num,estimate_num);
ele_matrix = zeros(group_num,estimate_num);
value_matrix = ele_matrix;

pos_matrix = round(pos_matrix .* length(datastream)); %choose positions
%pos_matrix = randperm(unqieu(datastrea))

for i=1:length(datastream);
    key = datastream(i);%incoming elements
    for i1=1:group_num
        for i2=1:estimate_num
            if ele_matrix(i1,i2)==0 && i==pos_matrix(i1,i2)
                ele_matrix(i1,i2) = key;
                value_matrix(i1,i2) = 1;
            elseif ele_matrix(i1,i2)>0 && ele_matrix(i1,i2) == key
                value_matrix(i1,i2) = value_matrix(i1,i2) + 1;
            end     
        end
    end
end

value_matrix = length(datastream).*(2.*value_matrix - 1);
group_average = zeros(1,group_num);
for i=1:group_num
    group_average(i) = sum(value_matrix(i,:))/length(value_matrix(i,:));
end
group_average = sort(group_average);
sec_moment_estimated = group_average(ceil(group_num/2));
err_sum = err_sum + (sec_moment_estimated-sec_moment)^2;
end

sqrt(err_sum)/sec_moment
    