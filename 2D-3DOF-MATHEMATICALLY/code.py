import math

L1 = 35
L2 = 22
L3 = 20

target_x = 50
target_z = 40
desired_orientation = 0

r_eff = math.sqrt(target_x**2 + target_z**2) - L3
z_eff = target_z

if r_eff < 0 or math.sqrt(r_eff**2 + z_eff**2) > L1 + L2:
    print("Target is not reachable.")
else:
    cos_theta2 = (r_eff**2 + z_eff**2 - L1**2 - L2**2) / (2 * L1 * L2)
    theta2 = math.acos(cos_theta2)

    theta1 = math.atan2(z_eff, r_eff) - math.atan2(L2 * math.sin(theta2), L1 + L2 * math.cos(theta2))

    theta3 = math.radians(desired_orientation) - (theta1 + theta2)

    theta1_deg = math.degrees(theta1)
    theta2_deg = math.degrees(theta2)
    theta3_deg = math.degrees(theta3)

    print(f"Shoulder (theta1): {theta1_deg:.2f} degrees")
    print(f"Elbow (theta2): {theta2_deg:.2f} degrees")
    print(f"Wrist (theta3): {theta3_deg:.2f} degrees")
