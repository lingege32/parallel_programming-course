clear;
% # of training data : # of testing data = 9:1
% first , do the normalization to increase accuracy
%parameter
ratio = 0.9;
training_num=1000*ratio;
testing_num = 1000-training_num;
str1='Class';
str2='/faceTrain';
str3='.bmp';
v_train = zeros(900,training_num*3);
v_test  = zeros(900,testing_num*3);
sum_train=zeros(900,1);
sum_test =zeros(900,1);
numOflevel2 = 7;
para = 0.08;
input_x = zeros(1,3);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%load training data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for num=1:training_num
    num_str=int2str(num);
    path=[str1 '1' str2 '1_' num_str str3];%path
    temp=double(imread(path));
    temp_re=reshape(temp,[900,1]);
    v_train(:,(num-1)*3+1)=temp_re;
    sum_train=sum_train+temp_re;

    path=[str1 '2' str2 '2_' num_str str3];%path
    temp=double(imread(path));
    temp_re=reshape(temp,[900,1]);
    v_train(:,(num-1)*3+2)=temp_re;
    sum_train=sum_train+temp_re;

    path=[str1 '3' str2 '3_' num_str str3];%path
    temp=double(imread(path));
    temp_re=reshape(temp,[900,1]);
    v_train(:,(num-1)*3+3)=temp_re;
    sum_train=sum_train+temp_re;
end
%load trainging data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%load testing data
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for num=training_num+1:1000
    num_str=int2str(num);
    path=[str1 '1' str2 '1_' num_str str3];%path
    temp=double(imread(path));
    temp_re=reshape(temp,[900,1]);
    v_test(:,(num-training_num-1)*3+1)=temp_re;
    sum_test=sum_test+temp_re;

    path=[str1 '2' str2 '2_' num_str str3];%path
    temp=double(imread(path));
    temp_re=reshape(temp,[900,1]);
    v_test(:,(num-training_num-1)*3+2)=temp_re;
    sum_test=sum_test+temp_re;

    path=[str1 '3' str2 '3_' num_str str3];%path
    temp=double(imread(path));
    temp_re=reshape(temp,[900,1]);
    v_test(:,(num-training_num-1)*3+3)=temp_re;
    sum_test=sum_test+temp_re;
end
%load testing data

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%start normalization
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    mean_ = (sum_train)/3/training_num;
    clear sum_train;
    clear sum_test;
    std_train = zeros(900,1);
    std_test = zeros(900,1);

    for num=1:training_num*3
        std_train = std_train +(v_train(:,num)-mean_).^2;
    end
    for num=1:testing_num*3
        std_test = std_test +(v_test(:,num)-mean_).^2;
    end
    std_=(std_train)/3/training_num;
    clear std_test;
    clear std_train;


    %normalization formula = (data-mean)/std
    for num=1:training_num*3
        v_train(:,num)=(v_train(:,num)-mean_)./std_;
    end
    v_train_t = v_train'; %use for pca m*n  m data n dimention


    coff = pca(v_train_t);

    v_train_t = v_train_t * coff;
    v_train_pca = v_train_t(:,1:2);
    clear v_train_t;
    clear v_train;
%start normalization

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%start training%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
w_level1 = ones(3,numOflevel2);
w_level2 = ones(numOflevel2+1,3);  %initialize w to one
cannot_class = 0;
for num=1:2
    %class 1
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    target = [1,0,0];
    input_x = [v_train_pca((num-1)*3+1,:),1];  %the last 1 is x0   [ x2 x1 x0]
    input_a1 = input_x * w_level1; %input_a is 1 by 2  w = [w1 , w2]
    temp_z = sigmf(input_a1,[1 0]);
    input_z = [temp_z,1];
    input_a2 = input_z * w_level2 ; % w_level2 = [w1 w2 w3]
    sigma_exp = sum(exp(input_a2));
    output_y = exp(input_a2) / sigma_exp;
    if (max(output_y)==output_y(1))
        output_y = [1 0 0];
    elseif(max(output_y)==output_y(2))
        output_y = [ 0 1 0];
    elseif(max(output_y)==output_y(3))
        output_y= [0 0 1];
    else
        cannot_class=cannot_class+1;
    end
    delta_k = (output_y-target);   %use transport to calculate gradient of E = delta * input z
    diff_level2 = input_z' * delta_k;  %3 by 3
    derived_h = sigmf(input_a1,[1 0]).*(1 - sigmf(input_a1,[1 0])); %1 by 2
    sigma_w_deltak = w_level2(1:numOflevel2,:) * delta_k';           %2 by 1
    delta_j = derived_h .* sigma_w_deltak';
    diff_level1 = input_x' * delta_j;
    w_level1 = w_level1 - para * diff_level1;
    w_level2 = w_level2 - para * diff_level2;
    %class 2
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    target = [0,1,0];
    input_x = [v_train_pca((num-1)*3+2,:),1];  %the last 1 is x0   [ x2 x1 x0]
    input_a1 = input_x * w_level1; %input_a is 1 by 2  w = [w1 , w2]
    temp_z = sigmf(input_a1,[1 0]);
    input_z = [temp_z,1];
    input_a2 = input_z * w_level2 ; % w_level2 = [w1 w2 w3]
    sigma_exp = sum(exp(input_a2));
    output_y = exp(input_a2) / sigma_exp;
    if (max(output_y)==output_y(1))
        output_y = [1 0 0];
    elseif(max(output_y)==output_y(2))
        output_y = [ 0 1 0];
    elseif(max(output_y)==output_y(3))
        output_y= [0 0 1];
    else
        cannot_class=cannot_class+1;
    end
    delta_k = (output_y-target);   %use transport to calculate gradient of E = delta * input z
    diff_level2 = input_z' * delta_k;  %3 by 3
    derived_h = sigmf(input_a1,[1 0]).*(1 - sigmf(input_a1,[1 0])); %1 by 2
    sigma_w_deltak = w_level2(1:numOflevel2,:) * delta_k';           %2 by 1
    delta_j = derived_h .* sigma_w_deltak';
    diff_level1 = input_x' * delta_j;
    w_level1 = w_level1 - para * diff_level1;
    w_level2 = w_level2 - para * diff_level2;
    %class 3
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    target = [0,0,1];
    input_x = [v_train_pca((num-1)*3+3,:),1];  %the last 1 is x0   [ x2 x1 x0]
    input_a1 = input_x * w_level1; %input_a is 1 by 2  w = [w1 , w2]
    temp_z = sigmf(input_a1,[1 0]);
    input_z = [temp_z,1];
    input_a2 = input_z * w_level2 ; % w_level2 = [w1 w2 w3]
    sigma_exp = sum(exp(input_a2));
    output_y = exp(input_a2) / sigma_exp;
    if (max(output_y)==output_y(1))
        output_y = [1 0 0];
    elseif(max(output_y)==output_y(2))
        output_y = [ 0 1 0];
    elseif(max(output_y)==output_y(3))
        output_y= [0 0 1];
    else
        cannot_class=cannot_class+1;
    end
    delta_k = (output_y-target);   %use transport to calculate gradient of E = delta * input z
    diff_level2 = input_z' * delta_k;  %3 by 3
    derived_h = sigmf(input_a1,[1 0]).*(1 - sigmf(input_a1,[1 0])); %1 by 2
    sigma_w_deltak = w_level2(1:numOflevel2,:) * delta_k';           %2 by 1
    delta_j = derived_h .* sigma_w_deltak';
    diff_level1 = input_x' * delta_j;
    w_level1 = w_level1 - para * diff_level1;
    w_level2 = w_level2 - para * diff_level2;
end


%%start training%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%start testing%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %normalization formula = (data-mean)/std

    for num=1:testing_num*3
        v_test(:,num)=(v_test(:,num)-mean_)./std_;
    end
    clear mean_;
    clear std_;
    v_test_t = v_test'; %use for pca m*n  m data n dimention

    v_test_t = v_test_t * coff;
    v_test_pca = v_test_t(:,1:2);
    clear v_test_t;
    clear v_test;

    error =0;
for num=1:testing_num
    %class 1
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    target = [1,0,0];
    input_x = [v_test_pca((num-1)*3+1,:),1];  %the last 1 is x0   [ x2 x1 x0]
    input_a1 = input_x * w_level1; %input_a is 1 by 2  w = [w1 , w2]
    temp_z = sigmf(input_a1,[1 0]);
    input_z = [temp_z,1];
    input_a2 = input_z * w_level2 ; % w_level2 = [w1 w2 w3]
    sigma_exp = sum(exp(input_a2));
    output_y = exp(input_a2) / sigma_exp;
    if (max(output_y)==output_y(1))
        output_y = [1 0 0];
    elseif(max(output_y)==output_y(2))
        output_y = [0 1 0];
    elseif(max(output_y)==output_y(3))
        output_y= [0 0 1];
    else
        cannot_class=cannot_class+1;
    end
    if(all(target(:)== output_y(:)))
        error = error ;
    else
        error = error +1;
    end

    %class 2
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    target = [0,1,0];
    input_x = [v_test_pca((num-1)*3+2,:),1] ; %the last 1 is x0   [ x2 x1 x0]
    input_a1 = input_x * w_level1; %input_a is 1 by 2  w = [w1 , w2]
    temp_z = sigmf(input_a1,[1 0]);
    input_z = [temp_z,1];
    input_a2 = input_z * w_level2 ; % w_level2 = [w1 w2 w3]
    sigma_exp = sum(exp(input_a2));
    output_y = exp(input_a2) / sigma_exp;
    if (max(output_y)==output_y(1))
        output_y = [1 0 0];
    elseif(max(output_y)==output_y(2))
        output_y = [ 0 1 0];
    elseif(max(output_y)==output_y(3))
        output_y= [0 0 1];
    else
        cannot_class=cannot_class+1;
    end
    if(all(target(:)== output_y(:)))
        error = error ;
    else
        error = error +1;
    end

    %class 3
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    target = [0,0,1];
    input_x = [v_test_pca((num-1)*3+3,:),1];  %the last 1 is x0   [ x2 x1 x0]
    input_a1 = input_x * w_level1; %input_a is 1 by 2  w = [w1 , w2]
    temp_z = sigmf(input_a1,[1 0]);
    input_z = [temp_z,1];
    input_a2 = input_z * w_level2 ; % w_level2 = [w1 w2 w3]
    sigma_exp = sum(exp(input_a2));
    output_y = exp(input_a2) / sigma_exp;
    if (max(output_y)==output_y(1))
        output_y = [1 0 0];
    elseif(max(output_y)==output_y(2))
        output_y = [ 0 1 0];
    elseif(max(output_y)==output_y(3))
        output_y= [0 0 1];
    else
        cannot_class=cannot_class+1;
    end
    if(all(target(:)== output_y(:)))
        error = error ;
    else
        error = error +1;
    end

end
%%start testing%%

accuracy = 1-(error/3/testing_num);



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%boudary decesion graph
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%the data after pca and normalization is between -1 to 1
class1=[];
class2=[];
class3=[];
for x = -1:0.02:1
    for y=-1:0.02:1
        input_x = [ x y 1];

        input_a1 = input_x * w_level1; %input_a is 1 by 2  w = [w1 , w2]
        temp_z = sigmf(input_a1,[1 0]);
        input_z = [temp_z,1];
        input_a2 = input_z * w_level2 ; % w_level2 = [w1 w2 w3]
        sigma_exp = sum(exp(input_a2));
        output_y = exp(input_a2) / sigma_exp;
        if (max(output_y)==output_y(1))
            class1=[class1 x y];
        elseif(max(output_y)==output_y(2))
            class2=[class2 x y];
        elseif(max(output_y)==output_y(3))
            class3=[class3 x y];
        else
            cannot_class=cannot_class+1;
        end
    end
end

figure(2);
scatter(class1(1,1:2:end),class1(1,2:2:end),'*');
hold on;
scatter(class2(1,1:2:end),class2(1,2:2:end),'*');
hold on;
scatter(class3(1,1:2:end),class3(1,2:2:end),'*');
legend('class1','class2','class3');
title('(a) boudary decision');
