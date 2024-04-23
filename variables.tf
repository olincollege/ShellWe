variable "region" {
  description = "The GCP region in which the resources will be created."
  type        = string
  default     = "us-east1"
}

variable "zone" {
  description = "The zone where the resources will reside."
  type        = string
  default     = "us-east1-c"
}

variable "image" {
  description = "The image for compute instance."
  type        = string
  default     = "ubuntu-os-cloud/ubuntu-2204-lts"
}
variable "project_id" {
  description = "The GCP project ID."
  type        = string
}

variable "credentials_loc" {
  description = "The GCP credential file."
  type        = string
}

variable "instance_type" {
  description = "The type of compute instance used to create the instance."
  type        = string
  default     = "e2-micro"
}
