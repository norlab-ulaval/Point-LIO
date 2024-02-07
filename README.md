# Point-LIO
## 1. Introduction

**Please try the new branch 'point-lio-with-grid-map', it solves some implementation problems of the original code, and also supports the simulation dataset with points of a scan collected at the same timestamp.**
<div align="center">
    <div align="center">
        <img src="https://github.com/hku-mars/Point-LIO/raw/master/image/toc4.png" width = 75% >
    </div>
    <font color=#a0a0a0 size=2>The framework and key points of the Point-LIO.</font>
</div>

**New features:**
1. would not fly under degeneration.
2. high odometry output frequency, 4k-8kHz.
3. robust to IMU saturation and severe vibration, and other aggressive motions (75 rad/s in our test).
4. no motion distortion.
5. computationally efficient, robust, versatile on public datasets with general motions. 
6. As an odometry, Point-LIO could be used in various autonomous tasks, such as trajectory planning, control, and perception, especially in cases involving very fast ego-motions (e.g., in the presence of severe vibration and high angular or linear velocity) or requiring high-rate odometry output and mapping (e.g., for high-rate feedback control and perception).

**Important notes:**

A. Please make sure the IMU and LiDAR are **Synchronized**, that's important.

B. Please obtain the saturation values of your used IMU (i.e., accelerator and gyroscope), and the units of the accelerator of your used IMU, then modify the .yaml file according to those settings, including values of 'satu_acc', 'satu_gyro', 'acc_norm'. That's improtant.

C. The warning message "Failed to find match for field 'time'." means the timestamps of each LiDAR points are missed in the rosbag file. That is important because Point-LIO processes at the sampling time of each LiDAR point.

D. We recommend to set the **extrinsic_est_en** to false if the extrinsic is given. As for the extrinsic initiallization, please refer to our recent work: [**Robust and Online LiDAR-inertial Initialization**](https://github.com/hku-mars/LiDAR_IMU_Init).

E. If a high odometry output frequency without downsample is required, set ``` publish_odometry_without_downsample ``` as true. Then the warning message of tf "TF_REPEATED_DATA" will pop up in the terminal window, because the time interval between two publish odometery is too small. The following command could be used to suppress this warning to a smaller frequency:

in your ros2_ws/src,

git clone --branch throttle-tf-repeated-data-error git@github.com:BadgerTechnologies/geometry2.git

Then rebuild, source setup.bash, run and then it should be reduced down to once every 10 seconds. If 10 seconds is still too much log output then change the ros::Duration(10.0) to 10000 seconds or whatever you like.

F. If you want to use Point-LIO without imu, set the "imu_en" as false, and provide a predefined value of gavity in "gravity_init" as true as possible in the yaml file, and keep the "use_imu_as_input" as false.

## **1.1. Developers:**
The codes of this repo are contributed by:
[Dongjiao He (贺东娇)](https://github.com/Joanna-HE) and [Wei Xu (徐威)](https://github.com/XW-HKU)


## **1.2. Related paper**
Our paper is published on Advanced Intelligent Systems(AIS). [Point-LIO](https://onlinelibrary.wiley.com/doi/epdf/10.1002/aisy.202200459), DOI: 10.1002/aisy.202200459


## **1.3. Related video**
Our accompany video is available on **YouTube**.
<div align="center">
    <a href="https://youtu.be/oS83xUs42Uw" target="_blank"><img src="https://github.com/hku-mars/Point-LIO/raw/master/image/final.png" width=60% /></a>
</div>

## 2. What can Point-LIO do?
### 2.1 Simultaneous LiDAR localization and mapping (SLAM) without motion distortion

### 2.2 Produce high odometry output frequence and high bandwidth

### 2.3 SLAM with aggressive motions even the IMU is saturated

# **3. Prerequisites**

## **3.1 Ubuntu and [ROS](https://www.ros.org/)**
We tested our code on Ubuntu22.04 with humble. Additional ROS package is required:
```
sudo apt-get install ros-xxx-pcl-conversions
```

## **3.2 Eigen**
Following the official [Eigen installation](eigen.tuxfamily.org/index.php?title=Main_Page), or directly install Eigen by:
```
sudo apt-get install libeigen3-dev
```

## 4. Build
Clone the repository and colcon build:

```
    cd ~/ros2_ws/src
    git clone https://github.com/norlab-ulaval/Point-LIO.git
    cd ..
    colcon build
    source install/setup.bash
```
- If you want to use a custom build of PCL, add the following line to ~/.bashrc
```export PCL_ROOT={CUSTOM_PCL_PATH}```

## 5. Directly run

### 5.1 For Rslidar

Step A: Setup before run

Edit ``` config/rslidar16.yaml ``` to set the below parameters:

1. LiDAR point cloud topic name: ``` lid_topic ```
2. IMU topic name: ``` imu_topic ``` (both internal and external, 6-aixes or 9-axies are fine)
3. Set the parameter ```timestamp_unit``` based on the unit of **t** field in PoindCloud2 rostopic
4. Line number (we tested 16, 32 and 64 line, but not tested 128 or above): ``` scan_line ```
5. Translational extrinsic: ``` extrinsic_T ```
6. Rotational extrinsic: ``` extrinsic_R ``` (only support rotation matrix)
- The extrinsic parameters in Point-LIO is defined as the LiDAR's pose (position and rotation matrix) in IMU body frame (i.e. the IMU is the base frame).
7. Saturation value of IMU's accelerator and gyroscope: ```satu_acc```, ```satu_gyro```
8. The norm of IMU's acceleration according to unit of acceleration messages: ``` acc_norm ```

Step B: Run below
```
    cd ~/ros2_ws
    source install/setup.bash
    ros2 launch point_lio rslidar16_launch.xml
```

Step C: Run LiDAR's ros driver or play rosbag.

### 5.2 PCD file save

Set ``` pcd_save_enable ``` in launchfile to ``` true ```. All the scans (in global frame) will be accumulated and saved to the file ``` Point-LIO/PCD/scans.pcd ``` after the Point-LIO is terminated. ```pcl_viewer scans.pcd``` can visualize the point clouds.

*Tips for pcl_viewer:*
- change what to visualize/color by pressing keyboard 1,2,3,4,5 when pcl_viewer is running. 
```
    1 is all random
    2 is X values
    3 is Y values
    4 is Z values
    5 is intensity
```

# **6. Examples**

The example datasets could be downloaded through [onedrive](https://connecthkuhk-my.sharepoint.com/:f:/g/personal/hdj65822_connect_hku_hk/EmRJYy4ZfAlMiIJ786ogCPoBcGQ2BAchuXjE5oJQjrQu0Q?e=igu44W). Pay attention that if you want to test on racing_drone.bag, [0.0, 9.810, 0.0] should be input in 'mapping/gravity_init' in avia.yaml, and set the 'start_in_aggressive_motion' as true in the yaml. Because this bag start from a high speed motion. And for PULSAR.bag, we change the measuring range of the gyroscope of the built-in IMU to 17.5 rad/s. Therefore, when you test on this bag, please change 'satu_gyro' to 17.5 in avia.yaml.

## **6.1. Example-1: SLAM on datasets with aggressive motions where IMU is saturated**
<div align="center">
<img src="https://github.com/hku-mars/Point-LIO/raw/master/image/example1.gif"  width="40%" />
<img src="https://github.com/hku-mars/Point-LIO/raw/master/image/example2.gif"  width="54%" />
</div>

## **6.2. Example-2: Application on FPV and PULSAR**
<div align="center">
<img src="https://github.com/hku-mars/Point-LIO/raw/master/image/example3.gif"  width="58%" />
<img src="https://github.com/hku-mars/Point-LIO/raw/master/image/example4.gif"  width="35%" />
</div>

PULSAR is a self-rotating UAV actuated by only one motor, [PULSAR](https://github.com/hku-mars/PULSAR)

## 7. Contact us
If you have any questions about this work, please feel free to contact me <hdj65822ATconnect.hku.hk> and Dr. Fu Zhang <fuzhangAThku.hk> via email.
