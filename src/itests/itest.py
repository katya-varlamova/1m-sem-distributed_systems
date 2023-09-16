import unittest
import json
import requests
URL = "http://127.0.0.1:8000/api/v1/persons"
p1 = {"name" : "katya",
      "age" : 22,
      "address" : "moscow",
      "work" : "huawei"}
p2 = {"name" : "katya_2",
      "age" : 23,
      "address" : "moscow_2",
      "work" : "huawei_2"}
p3 = {"name" : "katya_3",
      "age" : 24,
      "address" : "moscow_3",
      "work" : "huawei_3"
      }
p_upd = {"name" : "katya_u",
         "age" : 25,
         "address" : "moscow_u",
         "work" : "huawei_u"
                   }
p_inv = {"nane" : "katya",
         "work" : "huawei"}
p1j = json.dumps(p1)
p2j = json.dumps(p2)
p3j = json.dumps(p3)
p_updj = json.dumps(p_upd)
p_invj = json.dumps(p_inv)
headers = {'Content-type': 'application/json', 'Accept': 'text/'}
class TestMain(unittest.TestCase):
  # positives
  def test_crd(self):
    pid = requests.post(URL, headers=headers, data = p1j).json()["id"]
    data = requests.get(URL + "/" + str(pid), headers=headers).json()

    status = requests.delete(URL + "/" + str(pid), headers=headers).status_code
    exp = status == 204 and data["name"] == p1["name"] and data["age"] == p1["age"] and data["address"] == p1["address"] and data["work"] == p1["work"]
    self.assertEqual(exp, True)
  def test_get_all(self):
    id1 = requests.post(URL, headers=headers, data = p1j).json()["id"]
    id2 = requests.post(URL, headers=headers, data = p2j).json()["id"]
    id3 = requests.post(URL, headers=headers, data = p3j).json()["id"]
    
    data = requests.get(URL, headers=headers).json()
    exp = len(data) == 3

    requests.delete(URL + "/" + str(id1), headers=headers)
    requests.delete(URL + "/" + str(id2), headers=headers)
    requests.delete(URL + "/" + str(id3), headers=headers)
    self.assertEqual(exp, True)
  def test_upd(self):
    id1 = requests.post(URL, headers=headers, data = p1j).json()["id"]
    
    r = requests.patch(URL + "/" + str(id1), headers=headers, data = p_updj)
    data = r.json()
    exp = data["name"] == p_upd["name"] and data["age"] == p_upd["age"] and data["address"] == p_upd["address"] and data["work"] == p_upd["work"]

    requests.delete(URL + "/" + str(id1), headers=headers)
    self.assertEqual(exp, True)
# negative
  def test_get_neg(self):
    r = requests.get(URL + "/" + str(503737), headers=headers)
    exp = r.status_code == 404
    self.assertEqual(exp, True)
  def test_upd_inv_data(self):
    id1 = requests.post(URL, headers=headers, data = p1j).json()["id"]
    r = requests.patch(URL + "/" + str(id1), headers=headers, data = p_invj)
    exp = r.status_code == 400
    requests.delete(URL + "/" + str(id1), headers=headers)
    self.assertEqual(exp, True)
  def test_upd_inv_type_id(self):
    id1 = requests.post(URL, headers=headers, data = p1j).json()["id"]
    r = requests.patch(URL + "/" + "smth", headers=headers, data = p_updj)
    exp = r.status_code == 400
    requests.delete(URL + "/" + str(id1), headers=headers)
    self.assertEqual(exp, True)
  def test_upd_inv_id(self):
    r = requests.patch(URL + "/" + "4579827", headers=headers, data = p_updj)
    exp = r.status_code == 404
    self.assertEqual(exp, True)
  def test_del_inv_type_id(self):
    r = requests.delete(URL + "/" + "smth", headers=headers)
    exp = r.status_code == 400
    self.assertEqual(exp, True)
  def test_del_inv_id(self):
    r = requests.delete(URL + "/" + "4579827", headers=headers)
    exp = r.status_code == 404
    self.assertEqual(exp, True)
  def test_create_inv_data(self):
    r = requests.post(URL, headers=headers, data = p_invj)
    exp = r.status_code == 400
    self.assertEqual(exp, True)
  def test_get_inv_type_id(self):
    r = requests.get(URL + "/" + "smth", headers=headers)
    exp = r.status_code == 400
    self.assertEqual(exp, True)
  def test_get_inv_id(self):
    r = requests.get(URL + "/" + "4579827", headers=headers)
    exp = r.status_code == 404
if __name__ == "__main__":
  unittest.main()
    
