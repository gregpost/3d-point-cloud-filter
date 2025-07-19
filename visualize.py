import open3d as o3d
import numpy as np
import sys

if len(sys.argv) < 2:
    print("Usage: python visualize.py <filename>")
    sys.exit(1)

filename = sys.argv[1]

try:
    points = np.loadtxt(filename)
except Exception as e:
    print(f"Error reading file '{filename}': {e}")
    sys.exit(1)

# Create point cloud
pcd = o3d.geometry.PointCloud()
pcd.points = o3d.utility.Vector3dVector(points)

# Visualize
o3d.visualization.draw_geometries([pcd])
