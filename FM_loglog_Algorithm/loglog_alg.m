clear all;
close all;
clc;
fid = fopen('stream_for_fm.txt','r');
datastream = fscanf(fid,'%d');
datastream = datastream';
fclose(fid);
phi_constant = 0.39701;
num_of_ele = length(unique(datastream));%accurate num of elements

repeat_num = 10;
for iter=1:repeat_num
    iter
    num_of_groups = 1;%in order to get more accurate estimate, multiple groups should be used
    num_of_bucket = 1;
    num_of_bits_for_bucket = ceil(log2(num_of_bucket));
    estimates_array = zeros(num_of_groups,num_of_bucket); %recording the leftmost 0 in each group and each bucket
    distinct_num_array = zeros(1,num_of_groups); 
    distinct_num_array_new = zeros(1,num_of_groups);
    N = 2^25+1; %the value range of the elements in stream
    a_array = round(rand(1,num_of_groups)*N);
    b_array = round(rand(1,num_of_groups)*N);

    for i =1:length(datastream)
        ele = datastream(i);
        for i_1 =1:num_of_groups
            a = a_array(i_1);
            b = b_array(i_1);
            hash_value = mod(ele*a+b,N);
            binary_hash_value = dec2bin(hash_value); %convert the fingperpint to binary
            bucket_index = mod(hash_value,num_of_bucket)+1;
            new_binary_hash_value = binary_hash_value(1:length(binary_hash_value) ...
                - num_of_bits_for_bucket);

            r = 0;% length of the run of zeros strating from right
            for j=length(new_binary_hash_value):-1:1
                if new_binary_hash_value(j) == '0'
                    r = r + 1;
                else
                    break;
                end
            end
            r = r+1;
            
            estimates_array(i_1,bucket_index) = max(estimates_array(i_1,bucket_index),r);
        end
    end


    for i=1:num_of_groups
        distinct_num_array(i) = phi_constant*num_of_bucket*2^(sum(estimates_array(i,:))/num_of_bucket); 
        for i_1=1:num_of_bucket
            distinct_num_array_new(i) = distinct_num_array_new(i) + 2^(-estimates_array(i,i_1));%
        end
        distinct_num_array_new(i) = ((distinct_num_array_new(i))^(-1))*0.72134*num_of_bucket^2;

    end
    
    num_of_ele_estimated(iter) = sum(distinct_num_array)/length(distinct_num_array);
    num_of_ele_estimated_new(iter)= sum(distinct_num_array_new)/length(distinct_num_array_new);
end
error_sum = 0;
error_sum_new = 0;
for iter=1:repeat_num
    error_sum = error_sum + (num_of_ele_estimated(iter)-num_of_ele)^2/repeat_num;
    error_sum_new = error_sum_new + (num_of_ele_estimated_new(iter)-num_of_ele)^2/repeat_num;
end
num_of_ele
error_sum = error_sum^(0.5)
error_sum_new = error_sum_new^(0.5)