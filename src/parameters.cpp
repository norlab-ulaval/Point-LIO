#include "parameters.h"

bool is_first_frame = true;
double lidar_end_time = 0.0, first_lidar_time = 0.0, time_con = 0.0;
double last_timestamp_lidar = -1.0, last_timestamp_imu = -1.0;
int pcd_index = 0;

std::string lid_topic, imu_topic;
bool prop_at_freq_of_imu, check_satu, con_frame, cut_frame;
bool use_imu_as_input, space_down_sample, publish_odometry_without_downsample;
int  init_map_size, con_frame_num;
double match_s, satu_acc, satu_gyro, cut_frame_time_interval;
float  plane_thr;
double filter_size_surf_min, filter_size_map_min, fov_deg;
double cube_len; 
float  DET_RANGE;
bool   imu_en, gravity_align, non_station_start;
double imu_time_inte;
double laser_point_cov, acc_norm;
double vel_cov, acc_cov_input, gyr_cov_input;
double gyr_cov_output, acc_cov_output, b_gyr_cov, b_acc_cov;
double imu_meas_acc_cov, imu_meas_omg_cov; 
int    lidar_type, pcd_save_interval;
std::vector<double> gravity_init, gravity;
std::vector<double> extrinT;
std::vector<double> extrinR;
bool   runtime_pos_log, pcd_save_en, path_en, extrinsic_est_en = true;
bool   scan_pub_en, scan_body_pub_en;
shared_ptr<Preprocess> p_pre;
double time_lag_imu_to_lidar = 0.0;

void readParameters(const rclcpp::Node::SharedPtr nh)
{
  p_pre.reset(new Preprocess());
  nh->declare_parameter<bool>("prop_at_freq_of_imu", 1);
  nh->get_parameter("prop_at_freq_of_imu", prop_at_freq_of_imu);
  nh->declare_parameter<bool>("use_imu_as_input", 1);
  nh->get_parameter("use_imu_as_input", use_imu_as_input);
  nh->declare_parameter<bool>("check_satu", 1);
  nh->get_parameter("check_satu", check_satu);
  nh->declare_parameter<int>("init_map_size", 100);
  nh->get_parameter("init_map_size", init_map_size);
  nh->declare_parameter<bool>("space_down_sample", 1);
  nh->get_parameter("space_down_sample", space_down_sample);
  nh->declare_parameter<double>("mapping.satu_acc", 3.0);
  nh->get_parameter("mapping.satu_acc", satu_acc);
  nh->declare_parameter<double>("mapping.satu_gyro", 35.0);
  nh->get_parameter("mapping.satu_gyro", satu_gyro);
  nh->declare_parameter<double>("mapping.acc_norm", 1.0);
  nh->get_parameter("mapping.acc_norm", acc_norm);
  nh->declare_parameter<float>("mapping.plane_thr", 0.05f);
  nh->get_parameter("mapping.plane_thr", plane_thr);
  nh->declare_parameter<int>("point_filter_num", 2);
  nh->get_parameter("point_filter_num", p_pre->point_filter_num);
  nh->declare_parameter<std::string>("common.lid_topic", ".livox.lidar");
  nh->get_parameter("common.lid_topic", lid_topic);
  nh->declare_parameter<std::string>("common.imu_topic", ".livox.imu");
  nh->get_parameter("common.imu_topic", imu_topic);
  nh->declare_parameter<bool>("common.con_frame", false);
  nh->get_parameter("common.con_frame", con_frame);
  nh->declare_parameter<int>("common.con_frame_num", 1);
  nh->get_parameter("common.con_frame_num", con_frame_num);
  nh->declare_parameter<bool>("common.cut_frame", false);
  nh->get_parameter("common.cut_frame", cut_frame);
  nh->declare_parameter<double>("common.cut_frame_time_interval", 0.1);
  nh->get_parameter("common.cut_frame_time_interval", cut_frame_time_interval);
  nh->declare_parameter<double>("common.time_lag_imu_to_lidar", 0.0);
  nh->get_parameter("common.time_lag_imu_to_lidar", time_lag_imu_to_lidar);
  nh->declare_parameter<double>("filter_size_surf", 0.5);
  nh->get_parameter("filter_size_surf", filter_size_surf_min);
  nh->declare_parameter<double>("filter_size_map", 0.5);
  nh->get_parameter("filter_size_map", filter_size_map_min);
  nh->declare_parameter<double>("cube_side_length", 200);
  nh->get_parameter("cube_side_length", cube_len);
  nh->declare_parameter<float>("mapping.det_range", 300.f);
  nh->get_parameter("mapping.det_range", DET_RANGE);
  nh->declare_parameter<double>("mapping.fov_degree", 180);
  nh->get_parameter("mapping.fov_degree", fov_deg);
  nh->declare_parameter<bool>("mapping.imu_en", true);
  nh->get_parameter("mapping.imu_en", imu_en);
  nh->declare_parameter<bool>("mapping.start_in_aggressive_motion", false);
  nh->get_parameter("mapping.start_in_aggressive_motion", non_station_start);
  nh->declare_parameter<bool>("mapping.extrinsic_est_en", true);
  nh->get_parameter("mapping.extrinsic_est_en", extrinsic_est_en);
  nh->declare_parameter<double>("mapping.imu_time_inte", 0.005);
  nh->get_parameter("mapping.imu_time_inte", imu_time_inte);
  nh->declare_parameter<double>("mapping.lidar_meas_cov", 0.1);
  nh->get_parameter("mapping.lidar_meas_cov", laser_point_cov);
  nh->declare_parameter<double>("mapping.acc_cov_input", 0.1);
  nh->get_parameter("mapping.acc_cov_input", acc_cov_input);
  nh->declare_parameter<double>("mapping.vel_cov", 20);
  nh->get_parameter("mapping.vel_cov", vel_cov);
  nh->declare_parameter<double>("mapping.gyr_cov_input", 0.1);
  nh->get_parameter("mapping.gyr_cov_input", gyr_cov_input);
  nh->declare_parameter<double>("mapping.gyr_cov_output", 0.1);
  nh->get_parameter("mapping.gyr_cov_output", gyr_cov_output);
  nh->declare_parameter<double>("mapping.acc_cov_output", 0.1);
  nh->get_parameter("mapping.acc_cov_output", acc_cov_output);
  nh->declare_parameter<double>("mapping.b_gyr_cov", 0.0001);
  nh->get_parameter("mapping.b_gyr_cov", b_gyr_cov);
  nh->declare_parameter<double>("mapping.b_acc_cov", 0.0001);
  nh->get_parameter("mapping.b_acc_cov", b_acc_cov);
  nh->declare_parameter<double>("mapping.imu_meas_acc_cov", 0.1);
  nh->get_parameter("mapping.imu_meas_acc_cov", imu_meas_acc_cov);
  nh->declare_parameter<double>("mapping.imu_meas_omg_cov", 0.1);
  nh->get_parameter("mapping.imu_meas_omg_cov", imu_meas_omg_cov);
  nh->declare_parameter<double>("preprocess.blind", 1.0);
  nh->get_parameter("preprocess.blind", p_pre->blind);
  nh->declare_parameter<int>("preprocess.lidar_type", 1);
  nh->get_parameter("preprocess.lidar_type", lidar_type);
  nh->declare_parameter<int>("preprocess.scan_line", 16);
  nh->get_parameter("preprocess.scan_line", p_pre->N_SCANS);
  nh->declare_parameter<int>("preprocess.scan_rate", 10);
  nh->get_parameter("preprocess.scan_rate", p_pre->SCAN_RATE);
  nh->declare_parameter<int>("preprocess.timestamp_unit", 1);
  nh->get_parameter("preprocess.timestamp_unit", p_pre->time_unit);
  nh->declare_parameter<double>("mapping.match_s", 81);
  nh->get_parameter("mapping.match_s", match_s);
  nh->declare_parameter<bool>("mapping.gravity_align", true);
  nh->get_parameter("mapping.gravity_align", gravity_align);
  nh->declare_parameter<std::vector<double>>("mapping.gravity", std::vector<double>());
  nh->get_parameter("mapping.gravity", gravity);
  nh->declare_parameter<std::vector<double>>("mapping.gravity_init", std::vector<double>());
  nh->get_parameter("mapping.gravity_init", gravity_init);
  nh->declare_parameter<std::vector<double>>("mapping.extrinsic_T", std::vector<double>());
  nh->get_parameter("mapping.extrinsic_T", extrinT);
  nh->declare_parameter<std::vector<double>>("mapping.extrinsic_R", std::vector<double>());
  nh->get_parameter("mapping.extrinsic_R", extrinR);
  nh->declare_parameter<bool>("odometry.publish_odometry_without_downsample", false);
  nh->get_parameter("odometry.publish_odometry_without_downsample", publish_odometry_without_downsample);
  nh->declare_parameter<bool>("publish.path_en", true);
  nh->get_parameter("publish.path_en", path_en);
  nh->declare_parameter<bool>("publish.scan_publish_en", 1);
  nh->get_parameter("publish.scan_publish_en", scan_pub_en);
  nh->declare_parameter<bool>("publish.scan_bodyframe_pub_en", 1);
  nh->get_parameter("publish.scan_bodyframe_pub_en", scan_body_pub_en);
  nh->declare_parameter<bool>("runtime_pos_log_enable", 0);
  nh->get_parameter("runtime_pos_log_enable", runtime_pos_log);
  nh->declare_parameter<bool>("pcd_save.pcd_save_en", false);
  nh->get_parameter("pcd_save.pcd_save_en", pcd_save_en);
  nh->declare_parameter<int>("pcd_save.interval", -1);
  nh->get_parameter("pcd_save.interval", pcd_save_interval);
}
