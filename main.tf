# Import the Google Cloud provider
provider "google" {
  project     = var.project_id
  region      = var.region
  zone        = var.zone
  credentials = file(var.credentials_loc)
}

# Create a VPC
resource "google_compute_network" "chat_vpc" {
  name                    = "chat-vpc"
  auto_create_subnetworks = "false"
}

# Create a subnet
resource "google_compute_subnetwork" "chat_subnet" {
  name          = "chat-subnet"
  region        = var.region
  network       = google_compute_network.chat_vpc.self_link
  ip_cidr_range = "10.0.1.0/24"
}

# Reserve a Static External IP Address
resource "google_compute_address" "chat_static_ip" {
  name   = "chat-ip"
  region = var.region
}

# Create a firewall rule to allow SSH, TCP:12345
resource "google_compute_firewall" "chat_firewall" {
  name    = "chat-firewall"
  network = google_compute_network.chat_vpc.self_link

  allow {
    protocol = "tcp"
    ports    = ["22", "12345"]
  }

  source_ranges = ["0.0.0.0/0"]
}

# Create an e2-micro VM instance
resource "google_compute_instance" "chat_server" {
  name         = "chat-server"
  machine_type = var.instance_type
  zone         = var.zone

  boot_disk {
    initialize_params {
      image = var.image
    }
  }

  network_interface {
    subnetwork = google_compute_subnetwork.chat_subnet.self_link

    access_config {
      nat_ip = google_compute_address.chat_static_ip.address
    }
  }

  metadata_startup_script = file("${path.module}/setup_server.sh")
  tags                    = ["chat-server"]
}

# Output the instance IP Address
output "instance_public_ip" {
  value       = google_compute_instance.chat_server.network_interface[0].access_config[0].nat_ip
  description = "The public IP address of the VM instance."
}

# Output the Static IP Address
output "static_ip" {
  value       = google_compute_address.chat_static_ip.address
  description = "The static IP address assigned to the instance."
}
