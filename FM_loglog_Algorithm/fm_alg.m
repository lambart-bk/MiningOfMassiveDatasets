clear all;
close all;
clc;
fid = fopen('stream_for_fm.txt','r');
datastream = fscanf(fid,'%d');
datastream = datastream';
fclose(fid);

num_of_groups = 4;%in order to get more accurate estimate, multiple groups should be used
num_of_hash_functions_for_one_group = 4;%number of hash functions used for one group

N = 2^25+1; %the value range of the elements in stream


R_matrix = zeros(num_of_groups,num_of_hash_functions_for_one_group); %the matrxi of estimates
average_R_for_groups = zeros(1,num_of_groups);%maintain the average value for each group

accurate_num_ele = length(unique(datastream));

repeat_num = 10;
estimate_num_ele = zeros(1,repeat_num);
for iter = 1:repeat_num
    iter
    a_matrix = rand(num_of_groups,num_of_hash_functions_for_one_group); %the matrix of coefficients for hash functions
    b_matrix = rand(num_of_groups,num_of_hash_functions_for_one_group); %the matrix of coefficients for hash functions
    a_matrix = a_matrix.*N; %convert the coefficients to integers
    b_matrix = b_matrix.*N; %convert the coefficients to integers
    for i =1:length(datastream)
        ele = datastream(i);
        for i_1=1:num_of_groups
            for i_2=1:num_of_hash_functions_for_one_group
                a = a_matrix(i_1,i_2);
                b = b_matrix(i_1,i_2);
                hash_value = mod(a*ele + b,N);%round(N*(mod(rand_value*ele,1))); %produce the fingperprint of ele
                binary_hash_value = dec2bin(hash_value); %convert the fingperpint to binary
                r = 0;%the number of trailing 0s in the binary value
                for j=length(binary_hash_value):-1:1
                    if binary_hash_value(j) == '0'
                        r = r + 1;
                    else
                        break;
                    end
                end
                R_matrix(i_1,i_2) = max(r,R_matrix(i_1,i_2));
            end
        end    
    end
    %estimate_matrix = 2.^R_matrix; %get the matrix of estimates
    for i_1=1:num_of_groups
        average_R_for_groups(i_1) = sum(R_matrix(i_1,:))/num_of_hash_functions_for_one_group;
    end
    %estimate_matrix = 2.^average_estimate_for_groups;
    average_R_for_groups = sort(average_R_for_groups);
    estimated_R = 0;
    if mod(num_of_groups,2)>0
        estimated_R = average_R_for_groups(ceil(num_of_groups/2));
    else
        estimated_R = (average_R_for_groups(num_of_groups/2) + ...
           average_R_for_groups(1+num_of_groups/2))/2;
    end
    estimate_num_ele(iter) = 2^estimated_R;
end
error_sum = 0;
for iter=1:repeat_num
    error_sum = error_sum + (estimate_num_ele(iter)-accurate_num_ele)^2
end
error_sum = error_sum/repeat_num;
accurate_num_ele
error_sum = error_sum^(0.5)

